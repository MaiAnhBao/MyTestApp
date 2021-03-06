//
// Created by hnnguyen on 12/09/2016.
//

#ifndef MYAPPLICATION_STORE_H
#define MYAPPLICATION_STORE_H

#include <cstdint>
#include <jni.h>
#include <pthread.h>

#define STORE_MAX_CAPACITY 16

typedef enum {
    StoreType_Integer,
    StoreType_String,
    StoreType_Color,
    StoreType_IntegerArray,
    StoreType_StringArray,
    StoreType_ColorArray
} StoreType;

typedef union {
    int32_t mInteger;
    char* mString;
    jobject mColor;
    int32_t* mIntegerArray;
    char** mStringArray;
    jobject* mColorArray;
}StoreValue;

typedef struct {
    char* mKey;
    StoreType mType;
    StoreValue mValue;
    int32_t mLength;
} StoreEntry;

typedef struct {
    StoreEntry mEntries[STORE_MAX_CAPACITY];
    int32_t mLength;
} Store;

typedef struct {
    Store* mStore;
    JavaVM* mJavaVM;
    jobject mLock;
    pthread_t mThread;
    int32_t mRunning;
} StoreWatcher;

bool isEntryValid(JNIEnv* pEnv, StoreEntry* pEntry, StoreType pType);
StoreEntry* allocateEntry(JNIEnv* pEnv, Store* pStore, jstring pKey);
StoreEntry* findEntry(JNIEnv* pEnv, Store* pStore, jstring pKey);
void releaseEntryValue(JNIEnv* pEnv, StoreEntry* pEntry);

void throwInvalidTypeException(JNIEnv* pEnv);
void throwNotExistingKeyException(JNIEnv* pEnv);
void throwStoreFullException(JNIEnv* pEnv);

StoreWatcher* startWatcher(JavaVM* pJavaVM, Store* pStore, jobject pLock);
void stopWatcher(StoreWatcher* pWatcher);
void* runWatcher(void* pArgs);
void processEntry(StoreEntry* pEntry);
#endif //MYAPPLICATION_STORE_H
