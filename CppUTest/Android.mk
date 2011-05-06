LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    src/CppUTest/CommandLineArguments.cpp \
    src/CppUTest/CommandLineTestRunner.cpp \
    src/CppUTest/Failure.cpp \
    src/CppUTest/JUnitTestOutput.cpp \
    src/CppUTest/SimpleString.cpp \
    src/CppUTest/TestOutput.cpp \
    src/CppUTest/TestPlugin.cpp \
    src/CppUTest/TestRegistry.cpp \
    src/CppUTest/TestResult.cpp \
    src/CppUTest/Utest.cpp \
    src/CppUTest/Extensions/OrderedTest.cpp \
    src/CppUTest/Extensions/SimpleStringFromStdint.cpp \
    src/Platforms/Gcc/UtestPlatform.cpp

#Freepine: omit std::string to fix compilation error.
#LOCAL_SRC_FILES += src/CppUTest/Extensions/SimpleStringExtensions.cpp
#LOCAL_SRC_FILES += src/CppUTest/Nulls/NullJUnitTestOutput.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include 

LOCAL_SHARED_LIBRARIES := \
    libc \
    libcutils \
    libutils

LOCAL_CFLAGS += --use-cxa-atexit

ifneq ($(BUILD_WITH_MEMORY_LEAK_DETECTION),true)
    LOCAL_CFLAGS += -DDISABLE_MEMORYLEAK_PLUGIN \
                    -DUT_NEW_MACROS_DISABLED \
                    -DUT_NEW_OVERRIDES_DISABLED \
                    -DUT_MALLOC_MACROS_DISABLED
else
    LOCAL_SRC_FILES += src/CppUTest/MemoryLeakAllocator.cpp \
                       src/CppUTest/MemoryLeakDetector.cpp \
                       src/CppUTest/MemoryLeakWarningPlugin.cpp \
                       src/CppUTest/TestHarness_c.cpp
endif

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= libCppUTest
#LOCAL_PRELINK_MODULE := false

include $(BUILD_STATIC_LIBRARY)
