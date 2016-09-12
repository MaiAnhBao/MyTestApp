//
// Created by hnnguyen on 12/09/2016.
//
#include <jni.h>
#include "Store.h"
#include <cstdlib>
#include <cstring>

static Store gStore;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        abort();
    }
    gStore.mLength = 0;
    return JNI_VERSION_1_6;
}

JNIEXPORT jint JNICALL Java_com_example_hnnguyen_myapplication_Store_getCount
        (JNIEnv* pEnv, jobject pObject) {
    return gStore.mLength;
}

bool isEntryValid(JNIEnv* pEnv, StoreEntry* pEntry, StoreType pType) {
    return ((pEntry != NULL) && (pEntry->mType == pType));
}

StoreEntry* allocateEntry(JNIEnv* pEnv, Store* pStore, jstring pKey) {
    // If entry already exists in the store, releases its content
    // and keep its key.
    StoreEntry* entry = findEntry(pEnv, pStore, pKey);
    if (entry != NULL) {
        releaseEntryValue(pEnv, entry);
    }
    // If entry does not exist, create a new entry
    // right after the entries already stored.
    else {
        entry = pStore->mEntries + pStore->mLength;
    // Copies the new key into its final C string buffer.
        const char* tmpKey = pEnv->GetStringUTFChars(pKey, NULL);
        entry->mKey = new char[strlen(tmpKey) + 1];
        strcpy(entry->mKey, tmpKey);
        pEnv->ReleaseStringUTFChars(pKey, tmpKey);
        ++pStore->mLength;
    }
    return entry;
}

StoreEntry* findEntry(JNIEnv* pEnv, Store* pStore, jstring pKey) {
    StoreEntry* entry = pStore->mEntries;
    StoreEntry* entryEnd = entry + pStore->mLength;
    const char* tmpKey = pEnv->GetStringUTFChars(pKey, NULL);
    while ((entry < entryEnd) && (strcmp(entry->mKey,tmpKey) != 0)) {
        ++entry;
    }
    pEnv->ReleaseStringUTFChars(pKey,tmpKey);

    return (entry == entryEnd) ? NULL : entry;
}

void releaseEntryValue(JNIEnv* pEnv, StoreEntry* pEntry) {
    switch (pEntry->mType) {
        case StoreType_String:
            delete pEntry->mValue.mString;
            break;
        case StoreType_Color:
            pEnv->DeleteGlobalRef(pEntry->mValue.mColor);
            break;
    }
}

JNIEXPORT jstring JNICALL Java_com_example_hnnguyen_myapplication_Store_getString
        (JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_String)) {
    // Converts a C string into a Java String.
        return pEnv->NewStringUTF(entry->mValue.mString);
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_example_hnnguyen_myapplication_Store_setString
        (JNIEnv* pEnv, jobject pThis, jstring pKey, jstring pString) {
    // Turns the Java string into a temporary C string.
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL) {
        entry->mType = StoreType_String;
    // Copy the temporary C string into its dynamically allocated
    // final location. Then releases the temporary string.
        jsize stringLength = pEnv->GetStringUTFLength(pString);
        entry->mValue.mString = new char[stringLength + 1];
    // Directly copies the Java String into our new C buffer.
        pEnv->GetStringUTFRegion(pString, 0, stringLength,
                                 entry->mValue.mString);
    // Append the null character for string termination.
        entry->mValue.mString[stringLength] = '\0'; }
}

JNIEXPORT jint JNICALL
Java_com_example_hnnguyen_myapplication_Store_getInteger(JNIEnv *env, jobject instance,
                                                         jstring pKey) {
    StoreEntry* entry = findEntry(env, &gStore, pKey);
    if (isEntryValid(env, entry, StoreType_Integer)) {
        return entry->mValue.mInteger;
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL
Java_com_example_hnnguyen_myapplication_Store_setInteger(JNIEnv *env, jobject instance,
                                                         jstring pKey, jint pInt) {
    StoreEntry* entry = allocateEntry(env, &gStore, pKey);
    if (entry != NULL) {
        entry->mType = StoreType_Integer;
        entry->mValue.mInteger = pInt;
    }
}

JNIEXPORT jobject JNICALL
Java_com_example_hnnguyen_myapplication_Store_getColor(JNIEnv *env, jobject instance,
                                                       jstring pKey) {
//    const char *pKey = env->GetStringUTFChars(pKey_, 0);

    StoreEntry* entry = findEntry(env, &gStore, pKey);
    if (isEntryValid(env, entry, StoreType_Color)) {
        return entry->mValue.mColor;
    } else {
        return NULL;
    }

//    env->ReleaseStringUTFChars(pKey_, pKey);
}

JNIEXPORT void JNICALL
Java_com_example_hnnguyen_myapplication_Store_setColor(JNIEnv *env, jobject instance, jstring pKey,
                                                       jobject pColor) {
//    const char *pKey = env->GetStringUTFChars(pKey_, 0);

    // TODO
    StoreEntry* entry = allocateEntry(env, &gStore, pKey);
    if (entry != NULL) {
        entry->mType = StoreType_Color;
    // The Java Color is going to be stored on the native side.
    // Need to keep a global reference to avoid a potential
    // garbage collection after method returns.
        entry->mValue.mColor = env->NewGlobalRef(pColor);
    }

//    env->ReleaseStringUTFChars(pKey_, pKey);
}