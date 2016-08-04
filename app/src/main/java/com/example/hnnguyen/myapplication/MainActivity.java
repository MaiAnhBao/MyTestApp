package com.example.hnnguyen.myapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "Test";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
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
}
