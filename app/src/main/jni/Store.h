//
// Created by hnnguyen on 12/09/2016.
//

#ifndef MYAPPLICATION_STORE_H
#define MYAPPLICATION_STORE_H

#include <cstdint>
#include <jni.h>

#define STORE_MAX_CAPACITY 16

typedef enum {
    StoreType_Integer,
    StoreType_String,
    StoreType_Color,
} StoreType;

typedef union {
    int32_t mInteger;
    char* mString;
    jobject mColor;
}StoreValue;

typedef struct {
    char* mKey;
    StoreType mType;
    StoreValue mValue;
} StoreEntry;

typedef struct {
    StoreEntry mEntries[STORE_MAX_CAPACITY];
    int32_t mLength;
} Store;

bool isEntryValid(JNIEnv* pEnv, StoreEntry* pEntry, StoreType pType);
StoreEntry* allocateEntry(JNIEnv* pEnv, Store* pStore, jstring pKey);
StoreEntry* findEntry(JNIEnv* pEnv, Store* pStore, jstring pKey);
void releaseEntryValue(JNIEnv* pEnv, StoreEntry* pEntry);
#endif //MYAPPLICATION_STORE_H
