LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    om/CppUnitLite/Failure.cpp \
    om/CppUnitLite/SimpleString.cpp \
    om/CppUnitLite/Test.cpp \
    om/CppUnitLite/TestRegistry.cpp \
    om/CppUnitLite/TestResult.cpp

LOCAL_SHARED_LIBRARIES := \
    libc \
    libcutils \
    libutils

LOCAL_MODULE:= libcppunitlite
#LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
