package com.example.hnnguyen.myapplication;

import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.Toast;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;

public class MainActivity extends AppCompatActivity {

    private Socket socket;
    private String serverIPAddr = "192.168.81.102";
    private static final int SERVERPORT = 8082;
    public PrintWriter out;

    public static final String TAG = "Test";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Client client = new Client();
//        new Thread(new client.ClientThread()).run();
        new AsyncAction().execute("");
    }

    @Override
    protected void onResume() {
        Toast.makeText(MainActivity.this, "onResume", Toast.LENGTH_SHORT).show();
        super.onResume();
    }

    @Override
    protected void onPause() {
        Toast.makeText(MainActivity.this, "onPause", Toast.LENGTH_SHORT).show();
        super.onPause();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        Log.d(TAG,"onTouchEvent");
        float eventX = event.getX();
        float eventY = event.getY();

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                Toast.makeText(this,"DOWN " + Float.toString(eventX) + " "  +Float.toString(eventY),Toast.LENGTH_SHORT).show();
                break;
            case MotionEvent.ACTION_MOVE:
                Toast.makeText(this,"Move " + Float.toString(eventX) + " "  +Float.toString(eventY) ,Toast.LENGTH_SHORT).show();
                break;
            case MotionEvent.ACTION_UP:
                // nothing to do
                Toast.makeText(this,"Up " + Float.toString(eventX) + " "  +Float.toString(eventY),Toast.LENGTH_SHORT).show();
                break;
            default:
                return false;
        }

        return true;
    }

    private class AsyncAction extends AsyncTask<String, Void, Void> {
        @Override
        protected Void doInBackground(String... params) {
            try {
                InetAddress inetAddress = InetAddress.getByName(serverIPAddr);
                socket = new Socket(inetAddress,SERVERPORT);

            } catch (UnknownHostException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }

            return null;
        }
    }
}
