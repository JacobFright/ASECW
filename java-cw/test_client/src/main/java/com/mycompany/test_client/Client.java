/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


package com.mycompany.test_client;
import java.io.PrintStream;
import java.net.Socket;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
/**
 *
 * @author ntu-user
 */
public class Client {
    private DataOutputStream dataOut;

    public static void main(String[] args) throws IOException{
     
    
    String temp = "";
    Socket sock = new Socket("localhost", 8080);
        while (!temp.equals("logout")) {
            
            PrintStream pr = new PrintStream(sock.getOutputStream());
            
            System.out.println("Enter Something");
            InputStreamReader rd = new InputStreamReader(System.in);
            BufferedReader ed = new BufferedReader(rd);
            //System.out.println(ed);
            
                        
            temp = ed.readLine();
            System.out.println("this is the temp var: " + temp);
            pr.println(temp);
            
            
            BufferedReader gt = new BufferedReader(new InputStreamReader(sock.getInputStream()));
            
            String tm = gt.readLine();
            System.out.println(tm);
//            try
//             {
//                 dataOut.writeUTF(logStatus); // writing to the underlying output stream
//             }
//             // For handling errors while writing to output stream
//             catch(IOException io)
//             {
//                 System.out.println(io);
//             }
            
       
    }
        sock.close();
    }}