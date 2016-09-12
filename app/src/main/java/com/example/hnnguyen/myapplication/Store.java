package com.example.hnnguyen.myapplication;

/**
 * Created by hnnguyen on 12/09/2016.
 */
public class Store {
    public enum StoreType {
        Integer,
        String,
        Color
    }

    static {
        System.loadLibrary("Store");
    }

    public native int getCount();
    public native String getString(String pKey);
    public native void setString(String pKey, String pStr);
    public native int getInteger(String pKey);
    public native void setInteger(String pKey, int pInt);
    public native Color getColor(String pKey);
    public native void setColor(String pKey, Color pColor);
}
