package xyz.dyh;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

public class GreetingServer extends Thread {
    ServerSocket serverSocket;

    GreetingServer(int port) throws IOException {
        serverSocket = new ServerSocket(port);
        System.out.println("Server started on port: " + port + ".");
    }

    @Override
    public void run() {
        while (true) {
            try {
                Socket client = serverSocket.accept();
                System.out.println("Accepted a connection from " + client.getRemoteSocketAddress());
                Thread server = new HTTPServer(client);
                server.start();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }
}
