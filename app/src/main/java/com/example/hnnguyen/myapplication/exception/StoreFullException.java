package com.example.hnnguyen.myapplication.exception;

public class StoreFullException extends RuntimeException {
    public StoreFullException(String detailMessage) {
        super(detailMessage);
    }
}
