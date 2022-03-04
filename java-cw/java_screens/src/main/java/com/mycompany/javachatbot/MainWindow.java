package com.mycompany.javachatbot;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javax.swing.*;  

public class MainWindow{
    
    @FXML
    private Button btnLogout;

    @FXML
    private void on_btnLogout_clicked() throws IOException {
        String message = "LOGOUT:";
        sendToServer(message);
    }
    
    @FXML
    private void sendToServer(String message) throws IOException{
        Socket sock = new Socket("localhost", 8080);
            
        PrintStream pr = new PrintStream(sock.getOutputStream());

        pr.println(message);
   
        BufferedReader gt = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            
        String results = gt.readLine().replace("[", "").replace("]","");
        
        String[] results_parts = results.split(":");
        if (results_parts[1].equals("1")){
            System.exit(0);
        }        
        
    }
    
}