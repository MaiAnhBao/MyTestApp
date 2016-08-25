package com.example.hnnguyen.myapplication;

import android.app.IntentService;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.provider.Telephony;

import com.example.hnnguyen.myapplication.data.SMSData;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by hnnguyen on 25/08/2016.
 */
public class readSMS extends IntentService {

    public readSMS(String name) {
        super(name);
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        List<SMSData> smsDataList = new ArrayList<SMSData>();
        Uri uri = Uri.parse("content://sms/inbox");
        Cursor cursor = getContentResolver().query(uri,null,null,null,null);

        while (cursor.moveToNext()) {
            SMSData smsData =  new SMSData();
            smsData.setBody(cursor.getString(cursor.getColumnIndexOrThrow("body")).toString());
            smsData.setNumber(cursor.getString(cursor.getColumnIndexOrThrow("address")).toString());
            smsDataList.add(smsData);
        }

        cursor.close();
    }
}
