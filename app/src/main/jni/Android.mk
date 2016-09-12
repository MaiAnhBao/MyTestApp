LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Store
LOCAL_SRC_FILES := Store.h Store.cpp

include $(BUILD_SHARED_LIBRARY)