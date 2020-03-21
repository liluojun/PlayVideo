# This is the Android makefile for yuv.libyuv for NDK.
LOCAL_PATH:= $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE    := yuvl
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../localfile/libyuv.so #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)
include $(CLEAR_VARS)
LOCAL_MODULE    :=media
LOCAL_SRC_FILES := media_jni_NativeMethod.cpp
LOCAL_SHARED_LIBRARIES := yuvl
LOCAL_LDLIBS :=  -llog
include $(BUILD_SHARED_LIBRARY)
