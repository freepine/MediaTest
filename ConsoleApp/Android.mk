LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    mediatest.cpp 

LOCAL_SHARED_LIBRARIES := \
    libmedia \
    libc \
    libcutils \
    libutils

LOCAL_MODULE:= mediatest

include $(BUILD_EXECUTABLE)
