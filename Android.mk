LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    main.cpp \
    MediaPlayerTest.cpp \
    LogTestResult.cpp

LOCAL_C_INCLUDES:= \
    $(LOCAL_PATH)/cppunitlite/om/CppUnitLite/

LOCAL_SHARED_LIBRARIES := \
    libcppunitlite \
    libmedia \
    libutils

LOCAL_MODULE:= mediaplayer_unittest
include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
