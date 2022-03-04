package com.mycompany.test_client_server;

import java.io.IOException;

/**
 *
 * @author ntu-user
 */
public class Main {

    public static void main(String[] args) throws IOException {
        MyServer server = new MyServer();
        server.startServer();
    }
}
