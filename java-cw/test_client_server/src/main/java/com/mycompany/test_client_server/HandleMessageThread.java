/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package com.mycompany.test_client_server;

import java.io.IOException;
import java.io.PrintStream;
import java.net.Socket;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author ntu-user
 */
public class HandleMessageThread {

    public Socket sock;

    public void setSock(Socket inputSock) {
        sock = inputSock;
    }

    public String user;

    public void setUser(String currentUser) {
        user = currentUser;
    }

    private String userPassword = "tRD9LVh0e7g5";
    private String url = "jdbc:mysql://127.0.0.1:3306/";
    private String dataBaseName = "Chats";
    private String userName = "admin";

    private List<String> login(String username, String password) {
        List<String> login = new ArrayList<String>();
        login.add("LOGIN");
        Integer userID = 0;
        String savedPwd = "";

        try {
            DriverManager.getConnection(url + dataBaseName, userName, userPassword);
            System.out.println("Database connection sucessful.");
        } catch (SQLException e) {
            e.printStackTrace();
        }

        String query = "SELECT UserID, Pword FROM Users WHERE Username = '" + username + "'";

        try {
            Connection conn = DriverManager.getConnection(url + dataBaseName, userName, userPassword);

            Statement statement = conn.createStatement();
            ResultSet resultSet = statement.executeQuery(query);

            while (resultSet.next()) {
                userID = resultSet.getInt(1);
                savedPwd = resultSet.getString(2);

                System.out.println("Password recieved: " + savedPwd);
            }

            if (savedPwd == null ? password == null : savedPwd.equals(password)) {
                query = "UPDATE Users SET ISACTIVE = 1, LASTACTIVE = UTC_TIMESTAMP(), SocketID = '-2' WHERE USERID = '" + userID + "'";
                statement.execute(query);
                login.add(":1");
                System.out.println("User Logged in");
                return login;
            } else {

                login.add(":0");
                return login;
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return login;
        }
    }

    private List<String> logout(String username) {
        List<String> logout = new ArrayList<String>();
        logout.add("LOGOUT");

        try {
            DriverManager.getConnection(url + dataBaseName, userName, userPassword);
            System.out.println("Database connection sucessful.");
        } catch (SQLException e) {
            e.printStackTrace();
        }

        String query = "UPDATE Users SET IsActive = 0, SocketID = '-1' WHERE USERNAME = '" + user + "'";

        try {
            Connection conn = DriverManager.getConnection(url + dataBaseName, userName, userPassword);

            Statement statement = conn.createStatement();
            statement.execute(query);

            logout.add(":1");
        } catch (SQLException e) {
            e.printStackTrace();
            logout.add(":0");
            return logout;
        }
        return logout;
    }

    private List<String> regAccount(String name, String username, String password) {
        List<String> registered = new ArrayList<String>();
        registered.add("REGISTER");
        String url = "jdbc:mysql://127.0.0.1:3306/";
        String dataBaseName = "Chats";
        String userName = "admin";

        try {
            DriverManager.getConnection(url + dataBaseName, userName, userPassword);
            System.out.println("Database connection sucessful.");
        } catch (SQLException e) {
            e.printStackTrace();
        }

        String query = "SELECT Username FROM Users WHERE Username = '" + username + "'";
        try {
            Connection conn = DriverManager.getConnection(url + dataBaseName, userName, userPassword);

            Statement statement = conn.createStatement();
            ResultSet resultSet = statement.executeQuery(query);

            Integer size = 0;
            if (resultSet != null) {
                resultSet.last();
                size = resultSet.getRow();
            }

            if (size > 0) {
                System.out.println("Row is greater than 0");
                registered.add(":0");
                return registered;
            } else {
                System.out.println("Row is not greater than 0");
                query = "INSERT INTO Users (NAME, USERNAME, PWORD) VALUES ('" + name + "','" + username + "','" + password + "')";
                statement.execute(query);
                System.out.println("Data inserted");
                registered.add(":1");
                return registered;
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return registered;
        }

    }

    public void handleMessage(String data) throws IOException {
        String[] read = data.split(":");
        List<String> results = new ArrayList<String>();

        if ("REGISTER".equals(read[0])) {
            results = regAccount(read[1], read[2], read[3]);
            PrintStream pr = new PrintStream(sock.getOutputStream());
            pr.println(results);
        } else if ("LOGIN".equals(read[0])) {
            results = login(read[1], read[2]);
            PrintStream pr = new PrintStream(sock.getOutputStream());
            pr.println(results);
        } else if ("LOGOUT".equals(read[0])) {
            results = logout(user);
            PrintStream pr = new PrintStream(sock.getOutputStream());
            pr.println(results);
        }

    }
}
