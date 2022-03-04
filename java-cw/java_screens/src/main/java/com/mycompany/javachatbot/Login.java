package com.mycompany.javachatbot;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.net.Socket;
import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.event.ActionEvent;
import javax.swing.JOptionPane;

public class Login {

    @FXML
    private Button btnLogin, btnReg, btnCancel;
    @FXML
    private TextField txtUser, txtPwd;

    @FXML
    private void switchToSecondary() throws IOException {
        App.setRoot("secondary");
    }

    @FXML
    private void on_btnLogin_clicked(ActionEvent event) throws IOException {
        String message = "LOGIN:" + txtUser.getText() + ":" + txtPwd.getText();
        sendToServer(message, txtUser.getText());
    }

    @FXML
    private void on_btnReg_clicked() throws IOException {
        App.setRoot("registration");
    }

    @FXML
    private void sendToServer(String message, String user) throws IOException {
        Socket sock = new Socket("localhost", 8080);

        PrintStream pr = new PrintStream(sock.getOutputStream());
        pr.println(message);

        BufferedReader gt = new BufferedReader(new InputStreamReader(sock.getInputStream()));

        String results = gt.readLine().replace("[", "").replace("]", "");

        String[] result_parts = results.split(":");
        if (result_parts[1].equals("1")) {
            App.setRoot("mainWindow");
        } else {
            JOptionPane.showMessageDialog(null,
                    "Incorrect Password",
                    "Login",
                    JOptionPane.INFORMATION_MESSAGE);
            txtPwd.setText("");
            return;
        }

    }
}
