LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../Libraries)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp



define classes-cpp-files
$(patsubst jni/%,%,$(shell find $(LOCAL_PATH)/../../Classes -name "*.cpp")) \
$(patsubst jni/%,%,$(shell find $(LOCAL_PATH)/../../Classes -name "*.cc")) \
$(patsubst jni/%,%,$(shell find $(LOCAL_PATH)/../../Classes -name "*.c"))
endef

define libs-effcommon-cpp-files
$(patsubst jni/%,%,$(shell find $(LOCAL_PATH)/../../Libraries/eff_comm -name "*.cpp"))
endef

define libs-rapidxml-cpp-files
$(patsubst jni/%,%,$(shell find $(LOCAL_PATH)/../../Libraries/rapidxml -name "*.cpp"))
endef

define libs-socketx-cpp-files
$(patsubst jni/%,%,$(shell find $(LOCAL_PATH)/../../Libraries/socketx -name "*.cpp"))
endef



LOCAL_SRC_FILES := cocos2dcpp/main.cpp \
                   cocos2dcpp/PlatformFunction.cpp \
                   $(call classes-cpp-files) \
                   $(call libs-effcommon-cpp-files) \
                   $(call libs-rapidxml-cpp-files) \
                   $(call libs-socketx-cpp-files)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../Libraries \
                    $(LOCAL_PATH)/../../Libraries/google

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2d_lua_static
LOCAL_WHOLE_STATIC_LIBRARIES += protobuf_static
LOCAL_WHOLE_STATIC_LIBRARIES += libmobclickcpp
LOCAL_WHOLE_STATIC_LIBRARIES += bugly_crashreport_cocos_static
LOCAL_WHOLE_STATIC_LIBRARIES += bugly_agent_cocos_static_lua
include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,google)
$(call import-module,libmobclickcpp)
$(call import-module,bugly)
$(call import-module,bugly/lua)
$(call import-module,scripting/lua-bindings/proj.android)
