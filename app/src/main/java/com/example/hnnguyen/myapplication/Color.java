package com.example.hnnguyen.myapplication;

import android.text.TextUtils;

/**
 * Created by hnnguyen on 12/09/2016.
 */
public class Color {
    private int mColor;

    public Color(String pColor) {
        if (TextUtils.isEmpty(pColor)) {
            throw new IllegalArgumentException();
        }
        this.mColor = android.graphics.Color.parseColor(pColor);
    }

    @Override
    public String toString() {
        return String.format("#%06X",mColor);
    }
}
