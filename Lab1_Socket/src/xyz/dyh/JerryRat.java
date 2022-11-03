package xyz.dyh;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.File;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.file.Path;
import java.nio.file.Paths;

public class JerryRat {
    public static void main(String[] args) throws IOException {
        File configFile = new File("config.xml");
        if (!configFile.exists()) {
            System.out.println("Config file not found.");
            return;
        }
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        try {
            DocumentBuilder builder = factory.newDocumentBuilder();
            Document config = builder.parse(configFile);
            NodeList servers = config.getElementsByTagName("server");
            for (int i = 0; i < servers.getLength(); i++) {
                Node server = servers.item(i);
                NodeList serverConfig = server.getChildNodes();
                int port = 0;
                String root = null;
                for (int j = 0; j < serverConfig.getLength(); j++) {
                    Node item = serverConfig.item(j);
                    if (item.getNodeName().equals("port")) {
                        port = Integer.parseInt(item.getTextContent());
                    } else if (item.getNodeName().equals("root")) {
                        root = item.getTextContent();
                    }
                }
                if (root == null || port == 0) {
                    System.out.println("Wrong config.");
                    return;
                }

                Thread thread = new GreetingServer(port, root);
                thread.start();
            }
        } catch (Exception e) {
            System.out.println("Error while reading config file.");
            throw new RuntimeException(e);
        }
    }
}
