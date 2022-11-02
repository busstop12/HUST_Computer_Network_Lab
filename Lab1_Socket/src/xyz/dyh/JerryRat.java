package xyz.dyh;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Path;
import java.nio.file.Paths;

public class JerryRat {
    public static void main(String[] args) throws IOException {
        Thread thread = new GreetingServer(9900, "C:\\wwwroot");
        thread.start();
    }
}
