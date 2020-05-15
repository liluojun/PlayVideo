# This is the Android makefile for yuv.libyuv for NDK.
LOCAL_PATH:= $(call my-dir)

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
include $(CLEAR_VARS)
LOCAL_MODULE    := yuvl
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../localfile/armeabi-v7a/libyuv.so #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)
endif
ifeq ($(TARGET_ARCH_ABI), arm64-v8a)
include $(CLEAR_VARS)
LOCAL_MODULE    := yuvl
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../localfile/arm64-v8a/libyuv.so #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)
endif

include $(CLEAR_VARS)
LOCAL_MODULE    :=mediayuv
LOCAL_SRC_FILES := media_jni_NativeMethod.cpp
LOCAL_SHARED_LIBRARIES := yuvl
LOCAL_LDLIBS :=  -llog
include $(BUILD_SHARED_LIBRARY)
