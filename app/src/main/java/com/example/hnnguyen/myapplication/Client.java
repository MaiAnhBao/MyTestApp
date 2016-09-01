package com.example.hnnguyen.myapplication;

import android.util.Log;

import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * Created by hnnguyen on 30/08/2016.
 */
public class Client {
    private Socket socket;
    private static final int SERVERPORT = 8082;
    public static final String SERVER_IP = "192.168.81.102";

    class ClientThread implements Runnable {
        @Override
        public void run() {
            try {
                InetAddress serverAddr = InetAddress.getByName(SERVER_IP);
                socket = new Socket(serverAddr,SERVERPORT);
            } catch (UnknownHostException e) {
                Log.d("",e.toString());
            } catch (IOException e) {
                Log.d("",e.toString());
            }
        }
    }
}
