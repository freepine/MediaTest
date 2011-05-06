LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    main.cpp \
    LogTestPlugin.cpp \
    MemoryLeakPlugin.cpp \
    MallocInfo.cpp \
    TestingMediaPlayerListener.cpp \
    MediaPlayerTest.cpp 

#LOCAL_SRC_FILES += MediaExtractorTest.cpp

LOCAL_C_INCLUDES:= \
    $(LOCAL_PATH)/CppUTest/include/

LOCAL_STATIC_LIBRARIES += libCppUTest

LOCAL_SHARED_LIBRARIES := \
    libmedia \
    libutils \
    libbinder \
    libc

#LOCAL_SHARED_LIBRARIES += libstagefright

#CFLAGS are set to override malloc and free to get memory leak detection in C programs
#LOCAL_CFLAGS += -Dmalloc=cpputest_malloc -Dfree=cpputest_free
LOCAL_CFLAGS += -DDISABLE_MEMORYLEAK_PLUGIN

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= mediaplayer_cpputest
include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
