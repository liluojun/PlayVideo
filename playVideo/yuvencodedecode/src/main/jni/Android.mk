LOCAL_PATH := $(call my-dir)
MY_APP_JNI_ROOT := $(realpath $(LOCAL_PATH))
include $(call all-subdir-makefiles)

