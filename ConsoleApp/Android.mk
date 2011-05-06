LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    mediatest.cpp 

LOCAL_SHARED_LIBRARIES := \
    libmedia \
    libc \
    libcutils \
    libbinder \
    libsurfaceflinger_client \
    libutils

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= mediatest

include $(BUILD_EXECUTABLE)
