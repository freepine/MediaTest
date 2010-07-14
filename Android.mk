LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
    main.cpp \
    MediaPlayerTest.cpp

LOCAL_C_INCLUDES:= \
    $(LOCAL_PATH)/CppUTest/include/

#LOCAL_WHOLE_STATIC_LIBRARIES := libCppUTest

LOCAL_SHARED_LIBRARIES := \
    libmedia \
    libutils \
    libCppUTest

LOCAL_MODULE:= mediaplayer_cpputest
include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
