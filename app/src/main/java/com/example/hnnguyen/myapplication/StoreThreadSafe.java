package com.example.hnnguyen.myapplication;

import com.example.hnnguyen.myapplication.exception.InvalidTypeException;
import com.example.hnnguyen.myapplication.exception.NotExistingKeyException;

/**
 * Created by hnnguyen on 16/09/2016.
 */
public class StoreThreadSafe extends Store {
    protected static Object LOCK;

    public StoreThreadSafe(StoreListener mListener) {
        super(mListener);
    }

    @Override
    public int getCount() {
        synchronized (LOCK) {
            return super.getCount();
        }
    }

    @Override
    public int getInteger(String pKey) throws NotExistingKeyException, InvalidTypeException {
        synchronized (LOCK) {
            return super.getInteger(pKey);
        }
    }

    @Override
    public void setInteger(String pKey, int pInt) {
        synchronized (LOCK) {
            super.setInteger(pKey, pInt);
        }
    }

    @Override
    public void stopWatcher(long pPointer) {
        synchronized (LOCK) {
            super.stopWatcher(pPointer);
        }
    }

    @Override
    public String getString(String pKey) throws NotExistingKeyException, InvalidTypeException {
        synchronized (LOCK) {
            return super.getString(pKey);
        }
    }

    @Override
    public void setString(String pKey, String pStr) {
        synchronized (LOCK) {
            super.setString(pKey, pStr);
        }
    }

    @Override
    public Color getColor(String pKey) throws NotExistingKeyException, InvalidTypeException {
        synchronized (LOCK) {
            return super.getColor(pKey);
        }
    }

    @Override
    public void setColor(String pKey, Color pColor) {
        synchronized (LOCK) {
            super.setColor(pKey, pColor);
        }
    }

    @Override
    public int[] getIntegerArray(String pKey) {
        synchronized (LOCK) {
            return super.getIntegerArray(pKey);
        }
    }

    @Override
    public void setIntegerArray(String pKey, int[] pIntArray) {
        synchronized (LOCK) {
            super.setIntegerArray(pKey, pIntArray);
        }
    }

    @Override
    public String[] getStringArray(String pKey) {
        synchronized (LOCK) {
            return super.getStringArray(pKey);
        }
    }

    @Override
    public void setStringArray(String pKey, String[] pStringArray) {
        synchronized (LOCK) {
            super.setStringArray(pKey, pStringArray);
        }
    }

    @Override
    public Color[] getColorArray(String pKey) {
        synchronized (LOCK) {
            return super.getColorArray(pKey);
        }
    }

    @Override
    public void setColorArray(String pKey, Color[] pColorArray) {
        synchronized (LOCK) {
            super.setColorArray(pKey, pColorArray);
        }
    }
}
