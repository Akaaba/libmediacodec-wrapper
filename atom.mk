
LOCAL_PATH := $(call my-dir)

ifeq ("$(TARGET_OS)-$(TARGET_OS_FLAVOUR)","linux-android")

include $(CLEAR_VARS)
LOCAL_MODULE := libmediacodec-wrapper
LOCAL_DESCRIPTION := Android MediaCodec API wrapper library
LOCAL_CATEGORY_PATH := libs
LOCAL_SRC_FILES := \
    src/mcw.c \
    src/mcw_jni.c \
    src/mcw_ndk.c \
    src/mcw_log.c
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CONDITIONAL_LIBRARIES := OPTIONAL:libulog

include $(BUILD_LIBRARY)

endif
