package com.example.hnnguyen.myapplication;

import com.example.hnnguyen.myapplication.exception.InvalidTypeException;
import com.example.hnnguyen.myapplication.exception.NotExistingKeyException;

/**
 * Created by hnnguyen on 12/09/2016.
 */
public class Store implements StoreListener {
    private StoreListener mListener;

    public Store(StoreListener mListener) {
        this.mListener = mListener;
    }

    @Override
    public void onSuccess(int pValue) {
        mListener.onSuccess(pValue);
    }

    @Override
    public void onSuccess(String pValue) {
        mListener.onSuccess(pValue);
    }

    @Override
    public void onSuccess(Color pValue) {
        mListener.onSuccess(pValue);
    }

    public enum StoreType {
        Integer,
        String,
        Color,
        IntegerArray,
        StringArray,
        ColorArray
    }

    static {
        System.loadLibrary("Store");
    }

    public native int getCount();
    public native String getString(String pKey)  throws NotExistingKeyException, InvalidTypeException;
    public native void setString(String pKey, String pStr);
    public native int getInteger(String pKey) throws NotExistingKeyException, InvalidTypeException;
    public native void setInteger(String pKey, int pInt);
    public native Color getColor(String pKey)  throws NotExistingKeyException, InvalidTypeException;
    public native void setColor(String pKey, Color pColor);
    public native int[] getIntegerArray(String pKey);
    public native void setIntegerArray(String pKey, int[] pIntArray);
    public native String[] getStringArray(String pKey);
    public native void setStringArray(String pKey,String[] pStringArray);
    public native Color[] getColorArray(String pKey);
    public native void setColorArray(String pKey,Color[] pColorArray);
    public native long startWatcher();
    public native void stopWatcher(long pPointer);
}
