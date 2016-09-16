//
// Created by hnnguyen on 12/09/2016.
//
#include <jni.h>
#include "Store.h"
#include <cstdlib>
#include <unistd.h>

static Store gStore;
static jclass StringClass;
static jclass ColorClass;

static jmethodID MethodOnSuccessInt;
static jmethodID MethodOnSuccessString;
static jmethodID MethodOnSuccessColor;

static jobject gLock;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        abort();
    }
    // If returned class is null, an exception is raised by the VM.
    jclass StringClassTmp = env->FindClass("java/lang/String");
    if (StringClassTmp == NULL) abort();
    StringClass = (jclass) env->NewGlobalRef(StringClassTmp);
    env->DeleteLocalRef(StringClassTmp);
    jclass ColorClassTmp = env->FindClass("com/example/hnnguyen/myapplication/Color");
    if (ColorClassTmp == NULL) abort();
    ColorClass = (jclass) env->NewGlobalRef(ColorClassTmp);
    env->DeleteLocalRef(ColorClassTmp);

    // Caches methods.
    jclass StoreClass = env->FindClass("com/example/hnnguyen/myapplication/Store");
    if (StoreClass == NULL) abort();
    MethodOnSuccessInt = env->GetMethodID(StoreClass, "onSuccess","(I)V");
    if (MethodOnSuccessInt == NULL) abort();
    MethodOnSuccessString = env->GetMethodID(StoreClass, "onSuccess","(Ljava/lang/String;)V");
    if (MethodOnSuccessString == NULL) abort();
    MethodOnSuccessColor = env->GetMethodID(StoreClass, "onSuccess","(Lcom/packtpub/store/Color;)V");
    if (MethodOnSuccessColor == NULL) abort();
    env->DeleteLocalRef(StoreClass);

    gStore.mLength = 0;

    jclass ObjectClass = env->FindClass("java/lang/Object");
    if (ObjectClass == NULL) abort();
    jmethodID ObjectConstructor = env->GetMethodID(ObjectClass,"<init>","()V");
    if (ObjectConstructor == NULL) abort();
    jobject lockTmp = env->NewObject(ObjectClass, ObjectConstructor);
    env->DeleteLocalRef(ObjectClass);
    gLock = env->NewLocalRef(lockTmp);
    env->DeleteLocalRef(lockTmp);

    jclass StoreThreadSafeClass = env->FindClass("com/example/hnnguyen/myaaplication/StoreThreadSafe");
    if (StoreThreadSafeClass == NULL) abort();
    jfieldID lockField = env->GetStaticFieldID(StoreThreadSafeClass,"LOCK","Ljava/lang/Object;");
    if (lockField == NULL) abort();
    env->SetStaticObjectField(StoreThreadSafeClass, lockField, gLock);
    env->DeleteLocalRef(StoreThreadSafeClass);
    return JNI_VERSION_1_6;
}

JNIEXPORT jint JNICALL Java_com_example_hnnguyen_myapplication_Store_getCount
        (JNIEnv *pEnv, jobject pObject) {
    return gStore.mLength;
}

bool isEntryValid(JNIEnv *pEnv, StoreEntry *pEntry, StoreType pType) {
    if (pEntry == NULL) {
        throwNotExistingKeyException(pEnv);
    } else if (pEntry->mType != pType) {
        throwInvalidTypeException(pEnv);
    }
    return !pEnv->ExceptionCheck();
//    return ((pEntry != NULL) && (pEntry->mType == pType));
}

StoreEntry *allocateEntry(JNIEnv *pEnv, Store *pStore, jstring pKey) {
    // If entry already exists in the store, releases its content
    // and keep its key.
    StoreEntry *entry = findEntry(pEnv, pStore, pKey);
    if (entry != NULL) {
        releaseEntryValue(pEnv, entry);
    }
        // If entry does not exist, create a new entry
        // right after the entries already stored.
    else {
        if (pStore->mLength >= STORE_MAX_CAPACITY) {
            throwStoreFullException(pEnv);
            return NULL;
        }
        entry = pStore->mEntries + pStore->mLength;
        // Copies the new key into its final C string buffer.
        const char *tmpKey = pEnv->GetStringUTFChars(pKey, NULL);
        entry->mKey = new char[strlen(tmpKey) + 1];
        strcpy(entry->mKey, tmpKey);
        pEnv->ReleaseStringUTFChars(pKey, tmpKey);
        ++pStore->mLength;
    }
    return entry;
}

StoreEntry *findEntry(JNIEnv *pEnv, Store *pStore, jstring pKey) {
    StoreEntry *entry = pStore->mEntries;
    StoreEntry *entryEnd = entry + pStore->mLength;
    const char *tmpKey = pEnv->GetStringUTFChars(pKey, NULL);
    while ((entry < entryEnd) && (strcmp(entry->mKey, tmpKey) != 0)) {
        ++entry;
    }
    pEnv->ReleaseStringUTFChars(pKey, tmpKey);

    return (entry == entryEnd) ? NULL : entry;
}

void releaseEntryValue(JNIEnv *pEnv, StoreEntry *pEntry) {
    switch (pEntry->mType) {
        case StoreType_String:
            delete pEntry->mValue.mString;
            break;
        case StoreType_Color:
            pEnv->DeleteGlobalRef(pEntry->mValue.mColor);
            break;
        case StoreType_IntegerArray:
            delete[] pEntry->mValue.mIntegerArray;
            break;
        case StoreType_StringArray:
            // Destroys every C string pointed by the String arra
            // before releasing it.
            for (int32_t i = 0; i < pEntry->mLength; ++i) {
                delete pEntry->mValue.mStringArray[i];
            }
            delete[] pEntry->mValue.mStringArray;
            break;
        case StoreType_ColorArray:
// Unreferences every Id before releasing the Id array.
            for (int32_t i = 0; i < pEntry->mLength; ++i) {
                pEnv->DeleteGlobalRef(pEntry->mValue.mColorArray[i]);
            }
            delete[] pEntry->mValue.mColorArray;
            break;
    }
}

JNIEXPORT jstring JNICALL Java_com_example_hnnguyen_myapplication_Store_getString
        (JNIEnv *pEnv, jobject pThis, jstring pKey) {
    StoreEntry *entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_String)) {
        // Converts a C string into a Java String.
        return pEnv->NewStringUTF(entry->mValue.mString);
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL Java_com_example_hnnguyen_myapplication_Store_setString
        (JNIEnv *pEnv, jobject pThis, jstring pKey, jstring pString) {
    // Turns the Java string into a temporary C string.
    StoreEntry *entry = allocateEntry(pEnv, &gStore, pKey);
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
        entry->mValue.mString[stringLength] = '\0';
        pEnv->CallVoidMethod(pThis,MethodOnSuccessString, (jstring) pEnv->NewStringUTF(entry->mValue.mString));
    }
}

JNIEXPORT jint JNICALL
Java_com_example_hnnguyen_myapplication_Store_getInteger(JNIEnv *env, jobject instance,
                                                         jstring pKey) {
    StoreEntry *entry = findEntry(env, &gStore, pKey);
    if (isEntryValid(env, entry, StoreType_Integer)) {
        return entry->mValue.mInteger;
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL
Java_com_example_hnnguyen_myapplication_Store_setInteger(JNIEnv *env, jobject instance,
                                                         jstring pKey, jint pInt) {
    StoreEntry *entry = allocateEntry(env, &gStore, pKey);
    if (entry != NULL) {
        entry->mType = StoreType_Integer;
        entry->mValue.mInteger = pInt;
        env->CallVoidMethod(instance,MethodOnSuccessInt, (jint) entry->mValue.mInteger);
    }
}

JNIEXPORT jobject JNICALL
Java_com_example_hnnguyen_myapplication_Store_getColor(JNIEnv *env, jobject instance,
                                                       jstring pKey) {
//    const char *pKey = env->GetStringUTFChars(pKey_, 0);

    StoreEntry *entry = findEntry(env, &gStore, pKey);
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
    StoreEntry *entry = allocateEntry(env, &gStore, pKey);
    if (entry != NULL) {
        entry->mType = StoreType_Color;
        // The Java Color is going to be stored on the native side.
        // Need to keep a global reference to avoid a potential
        // garbage collection after method returns.
        entry->mValue.mColor = env->NewGlobalRef(pColor);
        env->CallVoidMethod(instance,MethodOnSuccessColor, (jstring) entry->mValue.mColor);
    }

//    env->ReleaseStringUTFChars(pKey_, pKey);
}

JNIEXPORT jintArray JNICALL
Java_com_example_hnnguyen_myapplication_Store_getIntegerArray(JNIEnv *pEnv, jobject instance,
                                                              jstring pKey) {
    StoreEntry *entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_IntegerArray)) {
        jintArray javaArray = pEnv->NewIntArray(entry->mLength);
        pEnv->SetIntArrayRegion(javaArray, 0, entry->mLength,
                                entry->mValue.mIntegerArray);
        return javaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL
Java_com_example_hnnguyen_myapplication_Store_setIntegerArray(JNIEnv *pEnv, jobject instance,
                                                              jstring pKey,
                                                              jintArray pIntegerArray) {
    StoreEntry *entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL) {
        jsize length = pEnv->GetArrayLength(pIntegerArray);
        int32_t *array = new int32_t[length];
        pEnv->GetIntArrayRegion(pIntegerArray, 0, length, array);
        entry->mType = StoreType_IntegerArray;
        entry->mLength = length;
        entry->mValue.mIntegerArray = array;
    }
}

JNIEXPORT jobjectArray JNICALL
Java_com_example_hnnguyen_myapplication_Store_getStringArray(JNIEnv *pEnv, jobject instance,
                                                             jstring pKey) {
    StoreEntry *entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_StringArray)) {
// An array of String in Java is in fact an array of object.
        jobjectArray javaArray = pEnv->NewObjectArray(entry->mLength,
                                                      StringClass, NULL);
// Creates a new Java String object for each C string stored.
// Reference to the String can be removed right after it is
// added to the Java array, as the latter holds a reference
// to the String object.
        for (int32_t i = 0; i < entry->mLength; ++i) {
            jstring string = pEnv->NewStringUTF(
                    entry->mValue.mStringArray[i]);
// Puts the new string in the array
            pEnv->SetObjectArrayElement(javaArray, i, string);
// Do it here to avoid holding many useless local refs.
            pEnv->DeleteLocalRef(string);
        }
        return javaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL
Java_com_example_hnnguyen_myapplication_Store_setStringArray(JNIEnv *pEnv, jobject instance,
                                                             jstring pKey,
                                                             jobjectArray pStringArray) {
    // Creates a new entry with the new String array.
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL) {
// Allocates an array of C string.
        jsize length = pEnv->GetArrayLength(pStringArray);
        char** array = new char*[length];
// Fills the C array with a copy of each input Java string.
        for (int32_t i = 0; i < length; ++i) {
// Gets the current Java String from the input Java array.
// Object arrays can be accessed element by element only.
            jstring string = (jstring)
                    pEnv->GetObjectArrayElement(pStringArray, i);
            jsize stringLength = pEnv->GetStringUTFLength(string);
            array[i] = new char[stringLength + 1];
// Directly copies the Java String into our new C buffer.
            pEnv->GetStringUTFRegion(string,0,stringLength, array[i]);
// Append the null character for string termination.
            array[i][stringLength] = '\0';
// No need to keep a reference to the Java string anymore.
            pEnv->DeleteLocalRef(string);
        }
        entry->mType = StoreType_StringArray;
        entry->mLength = length;
        entry->mValue.mStringArray = array;
    }
}

JNIEXPORT jobjectArray Java_com_example_hnnguyen_myapplication_Store_getColorArray(JNIEnv *pEnv, jobject instance,
                                                                                   jstring pKey) {
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_ColorArray)) {
// Creates a new array with objects of type Id.
        jobjectArray javaArray = pEnv->NewObjectArray(entry->mLength,
                                                      ColorClass, NULL);
// Fills the array with the Color objects stored on the native
// side, which keeps a global reference to them. So no need
// to delete or create any reference here.
        for (int32_t i = 0; i < entry->mLength; ++i) {
            pEnv->SetObjectArrayElement(javaArray, i,
                                        entry->mValue.mColorArray[i]);
        }
        return javaArray;
    } else {
        return NULL;
    }
}

JNIEXPORT void Java_com_example_hnnguyen_myapplication_Store_setColorArray(JNIEnv *pEnv, jobject instance,
                                                                           jstring pKey,
                                                                           jobjectArray pColorArray) {
    // Saves the Color array in the store.
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL) {
// Allocates a C array of Color objects.
        jsize length = pEnv->GetArrayLength(pColorArray);
        jobject* array = new jobject[length];
// Fills the C array with a copy of each input Java Color.
        for (int32_t i = 0; i < length; ++i) {
// Gets the current Color object from the input Java array.
            // Object arrays can be accessed element by element only.
            jobject localColor = pEnv->GetObjectArrayElement(
                    pColorArray, i);
// The Java Color is going to be stored on the native side
// Need to keep a global reference to avoid a potential
// garbage collection after method returns.
            array[i] = pEnv->NewGlobalRef(localColor);
// We have a global reference to the Color, so we can now
// get rid of the local one.
            pEnv->DeleteLocalRef(localColor);
        }
        entry->mType = StoreType_ColorArray;
        entry->mLength = length;
        entry->mValue.mColorArray = array;
    }
}

void throwInvalidTypeException(JNIEnv* pEnv) {
    jclass clazz = pEnv->FindClass("com/example/hnnguyen/myapplication/exception/InvalidTypeException");
    if (clazz != NULL) {
        pEnv->ThrowNew(clazz,"Invalid Key.");
    }
    pEnv->DeleteLocalRef(clazz);
}

void throwNotExistingKeyException(JNIEnv* pEnv) {
    jclass clazz = pEnv->FindClass("com/example/hnnguyen/myapplication/exception/NotExistingKeyException");
    if (clazz != NULL) {
        pEnv->ThrowNew(clazz,"Key does not exist.");
    }
    pEnv->DeleteLocalRef(clazz);
}

void throwStoreFullException(JNIEnv* pEnv) {
    jclass clazz = pEnv->FindClass("com/example/hnnguyen/myapplication/exception/StoreFullException");
    if (clazz != NULL) {
        pEnv->ThrowNew(clazz,"Store is full.");
    }
    pEnv->DeleteLocalRef(clazz);
}

JNIEXPORT jlong JNICALL
Java_com_example_hnnguyen_myapplication_Store_startWatcher(JNIEnv *env, jobject instance) {
    JavaVM* javaVM;
    if(env->GetJavaVM(&javaVM) != JNI_OK) abort();
    StoreWatcher* watcher = startWatcher(javaVM,&gStore,gLock);
    return (jlong) watcher;
}

JNIEXPORT void JNICALL
Java_com_example_hnnguyen_myapplication_Store_stopWatcher(JNIEnv *env, jobject instance,
                                                          jlong pPointer) {

    stopWatcher((StoreWatcher*) pPointer);

}

StoreWatcher* startWatcher(JavaVM* pJavaVM, Store* pStore, jobject pLock) {
    StoreWatcher* watcher = new StoreWatcher();
    watcher->mJavaVM = pJavaVM;
    watcher->mStore = pStore;
    watcher->mLock = pLock;
    watcher->mRunning = true;

    pthread_attr_t lAttributes;
    if (pthread_attr_init(&lAttributes)) abort();
    if (pthread_create(&watcher->mThread, &lAttributes, runWatcher, watcher)) abort();
    return watcher;
}

void stopWatcher(StoreWatcher* pWatcher) {
    pWatcher->mRunning = false;
}

void* runWatcher(void* pArgs) {
    StoreWatcher* storeWatcher = (StoreWatcher*) pArgs;
    Store* store = storeWatcher->mStore;
    JavaVM* javaVM = storeWatcher->mJavaVM;
    JavaVMAttachArgs javaVMAttachArgs;
    javaVMAttachArgs.version = JNI_VERSION_1_6;
    javaVMAttachArgs.name = "NativeThread";
    javaVMAttachArgs.group = NULL;

    JNIEnv* env;
    if (javaVM->AttachCurrentThreadAsDaemon(&env, &javaVMAttachArgs) != JNI_OK) abort();
    while (true) {
        sleep(5);
        env->MonitorEnter(storeWatcher->mLock);
        if (!storeWatcher->mRunning) break;
        StoreEntry* entry = storeWatcher->mStore->mEntries;
        StoreEntry* entryEnd = entry + storeWatcher->mStore->mLength;
        while (entry < entryEnd) {
            processEntry(entry);
            ++entry;
        }
        env->MonitorExit(storeWatcher->mLock);
    }
    javaVM->DetachCurrentThread();
    delete storeWatcher;
    pthread_exit(NULL);
}

void processEntry(StoreEntry* pEntry) {
    switch (pEntry->mType) {
        case StoreType_Integer:
            if (pEntry->mValue.mInteger > 100000) {
                pEntry->mValue.mInteger = 100000;
            } else if (pEntry->mValue.mInteger < -100000) {
                pEntry->mValue.mInteger = -100000;
            }
            break;
    }
}