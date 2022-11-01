package xyz.dyh;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;

public class HTTPServer extends Thread {
    Socket client;

    HTTPServer(Socket client) {
        this.client = client;
    }

    @Override
    public void run() {
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(client.getInputStream()));
            System.out.println(reader.readLine());
            client.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
