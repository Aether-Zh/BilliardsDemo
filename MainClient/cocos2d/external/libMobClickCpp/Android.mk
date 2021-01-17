LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libmobclickcpp

LOCAL_SRC_FILES := umengJniHelper.cpp\
    MobClickCpp.cpp

LOCAL_CFLAGS := 

LOCAL_EXPORT_CFLAGS := 

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)


LOCAL_LDLIBS := -landroid
LOCAL_LDLIBS += -llog

include $(BUILD_STATIC_LIBRARY)


