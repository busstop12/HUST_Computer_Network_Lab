package xyz.dyh;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public class JerryRat {
    public static void main(String[] args) throws IOException {
        Thread thread = new GreetingServer(9900);
        thread.start();
    }
}
