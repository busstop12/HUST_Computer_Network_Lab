package xyz.dyh;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Path;
import java.nio.file.Paths;

public class GreetingServer extends Thread {
    ServerSocket serverSocket;
    File rootDir;

    GreetingServer(int port, String root) throws IOException {
        serverSocket = new ServerSocket(port);
        System.out.println("Server started on port: " + port + ".");

        rootDir = new File(root);
        if (!rootDir.isDirectory()) {
            System.out.println("Error: Root " + root + " is not a directory.");
            this.interrupt();
        }
    }

    @Override
    public void run() {

        while (true) {
            if (this.isInterrupted()) return;
            try {
                Socket client = serverSocket.accept();
                Thread server = new HTTPServer(client, rootDir);
                server.start();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }
}
