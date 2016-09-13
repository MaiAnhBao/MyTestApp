package com.example.hnnguyen.myapplication.exception;

public class NotExistingKeyException extends Exception {
    public NotExistingKeyException(String detailMessage) {
        super(detailMessage);
    }
}

