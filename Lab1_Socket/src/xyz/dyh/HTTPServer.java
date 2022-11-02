package xyz.dyh;

import java.io.*;
import java.net.Socket;
import java.net.SocketException;
import java.nio.file.Path;

public class HTTPServer extends Thread {
    Socket client;
    File rootDir;

    HTTPServer(Socket client, File rootDir) {
        this.client = client;
        this.rootDir = rootDir;
    }

    @Override
    public void run() {
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(client.getInputStream()));
            String header = reader.readLine();
            if (header == null) {
                throw new HTTPException(400, "Bad Request");
            }

            String protocol = header.substring(header.lastIndexOf(' ') + 1);
            if (!protocol.equals("HTTP/1.1")) {
                throw new HTTPException(400, "Protocol not allowed.");
            }

            String method = header.substring(0, header.indexOf(' '));
            if (!method.equals("GET")) {
                throw new HTTPException(405, "Method: " + method + " not allowed.");
            }

            String path = header.substring(header.indexOf(' ') + 1, header.lastIndexOf(' '));
            sendFile(path);

            client.close();
        } catch (HTTPException e) {
            switch (e.status) {
                case 400 -> {
                    sendMessage(400, e.msg, "400 Bad Request");
                    log(400, "");
                }
                case 405 -> {
                    sendMessage(405, e.msg, "405 Method Not Allowed");
                    log(405, "");
                }
                default -> {
                    sendMessage(500, "Internal Server Error", "500 Internal Server Error");
                    log(500, "");
                    throw new RuntimeException(e);
                }
            }
        } catch (IOException e) {
            sendMessage(500, "Internal Server Error", "500 Internal Server Error");
            log(500, "");
            throw new RuntimeException(e);
        }
    }

    private void sendMessage(int status, String msg, String body) {
        PrintWriter out = null;
        try {
            out = new PrintWriter(client.getOutputStream());
        } catch (SocketException e) {
            return;
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        out.println("HTTP/1.0 " + status + ' ' + msg);
        out.println("Content_Type:text/html; charset=UTF-8");
        out.println("");
        out.println(body);
        out.close();
    }

    private void sendFile(String path) throws IOException {
        PrintStream out = new PrintStream(client.getOutputStream(), true);
        File file = new File(rootDir, path);
        if (file.isDirectory()) {
            file = new File(file, "index.html");
        }
        if (!file.exists()) {
            sendMessage(404, "Not found", "404 Not Found: " + path);
            log(404, path);
            return;
        }

        try {
            FileInputStream fin = new FileInputStream(file);
            byte[] data = new byte[fin.available()];
            int len = fin.read(data);
            String filename = file.getName();

            out.println("HTTP/1.0 200 OK");
            out.println("Content-Type: " + getContentType(filename.substring(filename.lastIndexOf('.') + 1)));
            out.println("Content-Length: " + len);
            out.println();
            out.write(data);

            fin.close();

            log(200, path);
        } catch (FileNotFoundException e) {
            sendMessage(404, "Not found", "404 Not Found: " + file.getPath());
        }

        out.close();
    }

    private String getContentType(String extension) {
        return switch (extension) {
            case "html" -> "text/html";
            case "css" -> "text/css";
            case "js" -> "text/javascript";
            case "jpg" -> "image/jpeg";
            case "png" -> "image/png";
            default -> "application/octet-stream";
        };

    }

    private void log(int status, String path) {
        String format = String.format("\033[1;%dm%d\033[0m\t%s\t%s", status == 200 ? 32 : 31, status, path, client.getRemoteSocketAddress());
        System.out.println(format);
    }
}