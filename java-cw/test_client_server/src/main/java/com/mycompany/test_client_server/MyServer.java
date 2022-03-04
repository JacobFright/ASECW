/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.test_client_server;

/**
 *
 * @author ntu-user
 */
import java.net.*;
import java.util.*;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class MyServer {

    public static void startServer() throws IOException {

        createDatabase();
        ServerSocket ser = new ServerSocket(8080);

        while (true) {

            Socket sock = ser.accept();

            BufferedReader ed = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            String tmp = ed.readLine();

            HandleMessageThread handleMessageThread = new HandleMessageThread();
            handleMessageThread.setSock(sock);
            handleMessageThread.handleMessage(tmp);

        }
    }

    public static void createDatabase() {
        String url = "jdbc:mysql://127.0.0.1:3306/";
        String dataConnectName = "information_schema";
        String dataBaseName = "Chats";
        String username = "admin";
        String password = "tRD9LVh0e7g5";

        // Initial connection to the database
        try {
            DriverManager.getConnection(url + dataConnectName, username, password);
            System.out.println("Database connection sucessful.");
        } catch (SQLException e) {
            e.printStackTrace();
        }

        // Inital creation of the 'Chats' database if it doesn't already exist
        String create = "CREATE DATABASE IF NOT EXISTS Chats";

        try {
            Connection conn = DriverManager.getConnection(url + dataConnectName, username, password);

            Statement statement = conn.createStatement();
            statement.execute(create);

            System.out.println("Chats database successfully created.");

        } catch (SQLException e) {
            e.printStackTrace();
        }

        //Initial connection to the 'Chats' database
        try {
            DriverManager.getConnection(url + dataBaseName, username, password);
            System.out.println("Connected to Chats database successful.");
        } catch (SQLException e) {
            e.printStackTrace();
        }

        List<String> tables = new ArrayList<String>();
        String[] tableNames = {"Users", "Chats", "Messages", "Participants", "Contacts"};

        tables.add("CREATE TABLE IF NOT EXISTS Users (UserID int NOT NULL AUTO_INCREMENT, Name varchar(50), Username varchar(50),"
                + " Pword varchar(50), IsActive TINYINT, LastActive datetime, SocketID INT, PRIMARY KEY (UserID));");
        tables.add("CREATE TABLE IF NOT EXISTS Chats (ChatID int NOT NULL AUTO_INCREMENT, Title varchar(255), PRIMARY KEY (ChatID));");
        tables.add("CREATE TABLE IF NOT EXISTS Messages (MessageID int NOT NULL AUTO_INCREMENT, SenderID int, ChatID int, SendTime datetime,"
                + "Message varchar(255),"
                + " PRIMARY KEY (MessageID), FOREIGN KEY (SenderID) REFERENCES Users(UserID),"
                + "FOREIGN KEY (ChatID) REFERENCES Chats(ChatID));");
        tables.add("CREATE TABLE IF NOT EXISTS Participants (ParticipantID int NOT NULL AUTO_INCREMENT, ChatID int, UserID int,"
                + " PRIMARY KEY (ParticipantID), FOREIGN KEY (ChatID) REFERENCES Chats(ChatID),"
                + "FOREIGN KEY (UserID) REFERENCES Users(UserID));");
        tables.add("CREATE TABLE IF NOT EXISTS Contacts (ContactID int NOT NULL AUTO_INCREMENT, ContactUserID int, UserID int,"
                + " PRIMARY KEY (ContactID),"
                + " FOREIGN KEY (ContactUserID) REFERENCES Users(UserID),"
                + "FOREIGN KEY (UserID) REFERENCES Users(UserID));");

        // Creation of database tables
        try {
            Connection conn = DriverManager.getConnection(url + dataBaseName, username, password);

            Statement statement = conn.createStatement();

            for (int i = 0; i < 5; i++) {
                statement.execute(tables.get(i));
                System.out.println("Table " + tableNames[i] + " created successfully.");
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }

        System.out.println("All tables created successfully");
    }
}
