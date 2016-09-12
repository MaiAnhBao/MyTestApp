package com.example.hnnguyen.myapplication;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import java.util.regex.Pattern;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "Test";
    private Store mStore = new Store();
    private EditText mUIKeyEditText, mUIValueEdit;
    private Spinner mUITypeSpinner;
    private Button mUIGetButton, mUISetButton;
    private Pattern mKeyPattern;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mKeyPattern = Pattern.compile("\\p{Alnum}+");
        mUIKeyEditText = (EditText) findViewById(R.id.uiKeyEdit);
        mUIValueEdit = (EditText) findViewById(R.id.uiValueEdit);

        ArrayAdapter<Store.StoreType> adapter = new ArrayAdapter<Store.StoreType>(MainActivity.this
                ,android.R.layout.simple_spinner_item, Store.StoreType.values());
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_item);
        mUITypeSpinner = (Spinner) findViewById(R.id.uiTypeSpinner);
        mUITypeSpinner.setAdapter(adapter);

        mUIGetButton = (Button) findViewById(R.id.uiGetValueButton);
        mUIGetButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onGetValue();
            }
        });

        mUISetButton = (Button) findViewById(R.id.uiSetValueButton);
        mUISetButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onSetValue();
            }
        });


        ((TextView) findViewById(R.id.txtWelcome)).setText(getMsgFromJni());



        Toast.makeText(this,getMsgFromJni(),Toast.LENGTH_LONG).show();
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


    private void onGetValue() {
        String key = mUIKeyEditText.getText().toString();
        Store.StoreType type = (Store.StoreType) mUITypeSpinner.getSelectedItem();
        if (!mKeyPattern.matcher(key).matches()) {
            displayMessage("Incorrect key.");
            return;
        }
        switch (type) {
            case Integer:
                mUIValueEdit.setText(Integer.toString(mStore.getInteger(key)));
                break;
            case String:
                mUIValueEdit.setText(mStore.getString(key));
                break;
            case Color:
                mUIValueEdit.setText(mStore.getColor(key).toString());
                break;
        }
    }

    private void onSetValue() {
        String key = mUIKeyEditText.getText().toString();
        String value = mUIValueEdit.getText().toString();
        Store.StoreType type = (Store.StoreType) mUITypeSpinner.getSelectedItem();
        if (!mKeyPattern.matcher(key).matches()) {
            displayMessage("Incorrect key.");
            return;
        }
        try {
            switch (type) {
                case Integer:
                    mStore.setInteger(key,Integer.parseInt(value));
                    break;
                case String:
                    mStore.setString(key,value);
                    break;
                case Color:
                    mStore.setColor(key,new Color(value));
                    break;
            }
        } catch (Exception e) {
            displayMessage("Incorrect value.");
        }

    }

    private void displayMessage(String pMessage) {
        Toast.makeText(MainActivity.this, pMessage, Toast.LENGTH_LONG)
                .show();
    }

    static {
        System.loadLibrary("client-socket-jni");
        System.loadLibrary("gpslocationprovider");
    }

    public native String getMsgFromJni();



}
