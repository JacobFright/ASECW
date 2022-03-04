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

public class Registration {
    
    @FXML
    private Button btnReg, btnCancel;
    @FXML
    private TextField txtUser, txtName, txtPwd, txtCfmPwd;

    @FXML
    private void switchToPrimary() throws IOException {
        App.setRoot("loginScreen");
    }
    
    @FXML
    private void on_btnCancel_clicked() throws IOException {
        App.setRoot("loginScreen");
    }
    
    @FXML
    private void on_btnReg_clicked()throws IOException {
        if (txtPwd.getText() == null ? txtCfmPwd.getText() != null : !txtPwd.getText().equals(txtCfmPwd.getText())){
            JOptionPane.showMessageDialog(null,
                "The passwords entered do not match",
                "Register",
                JOptionPane.INFORMATION_MESSAGE);
            txtCfmPwd.setText("");
            return;
        }
        String regDetails = "REGISTER:" + txtName.getText() + ":" + txtUser.getText() + ":" + txtPwd.getText();
        sendToServer(regDetails);
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
            JOptionPane.showMessageDialog(null,
                "Account Registered",
                "Register",
                JOptionPane.INFORMATION_MESSAGE);
            txtCfmPwd.setText("");
            
            App.setRoot("loginScreen");
        }
        else
        {
            JOptionPane.showMessageDialog(null,
                "Username in use",
                "Register",
                JOptionPane.INFORMATION_MESSAGE);
            txtCfmPwd.setText("");
            return;
        }
        
        
    }
    
}