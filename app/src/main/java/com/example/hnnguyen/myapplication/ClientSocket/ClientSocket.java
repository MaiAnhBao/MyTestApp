package com.example.hnnguyen.myapplication.ClientSocket;

import android.os.AsyncTask;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * Created by hnnguyen on 05/09/2016.
 */
public class ClientSocket extends AsyncTask<String, Void, Void> {
    private InetAddress inetAddress;
    private int PORT = 50007;
    Socket socket;
    @Override
    protected Void doInBackground(String... params) {
        try {
//            inetAddress = InetAddress.getByName("");
            socket = new Socket("192.168.1.19", PORT);
            BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));
            DataOutputStream outToServer = new DataOutputStream(socket.getOutputStream());
            String sentence = inFromUser.readLine();
            outToServer.writeBytes(sentence + '\n');
        } catch (UnknownHostException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}
