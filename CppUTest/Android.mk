LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    src/CppUTest/CommandLineArguments.cpp \
    src/CppUTest/CommandLineTestRunner.cpp \
    src/CppUTest/Failure.cpp \
    src/CppUTest/JUnitTestOutput.cpp \
    src/CppUTest/MemoryLeakAllocator.cpp \
    src/CppUTest/MemoryLeakDetector.cpp \
    src/CppUTest/MemoryLeakWarningPlugin.cpp \
    src/CppUTest/SimpleString.cpp \
    src/CppUTest/TestHarness_c.cpp \
    src/CppUTest/TestOutput.cpp \
    src/CppUTest/TestPlugin.cpp \
    src/CppUTest/TestRegistry.cpp \
    src/CppUTest/TestResult.cpp \
    src/CppUTest/Utest.cpp \
    src/CppUTest/Extensions/OrderedTest.cpp \
    src/CppUTest/Extensions/SimpleStringFromStdint.cpp \
    src/CppUTest/Nulls/NullJUnitTestOutput.cpp \
    src/Platforms/Gcc/UtestPlatform.cpp

#Freepine: omit std::string to fix compilation error.
#LOCAL_SRC_FILES += src/CppUTest/Extensions/SimpleStringExtensions.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include 

LOCAL_SHARED_LIBRARIES := \
    libc \
    libcutils \
    libutils

LOCAL_MODULE:= libCppUTest
#LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false

include $(BUILD_STATIC_LIBRARY)
