LOCAL_PATH := $(call my-dir)
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
include $(CLEAR_VARS)
LOCAL_MODULE    := avcodec    #指定一个当前模块名1
LOCAL_SRC_FILES :=$(LOCAL_PATH)/../../../../media/ffmpeg/armeabi-v7a/lib/libavcodec.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avfilter    #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/armeabi-v7a/lib/libavfilter.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avformat    #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/armeabi-v7a/lib/libavformat.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avutil   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/armeabi-v7a/lib/libavutil.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE    := postproc    #指定一个当前模块名1
#LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/armeabi-v7a/lib/libpostproc.so  #要编译的源文件
#include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := swresample   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/armeabi-v7a/lib/libswresample.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := swscale   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/armeabi-v7a/lib/libswscale.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)
endif

ifeq ($(TARGET_ARCH_ABI), arm64-v8a)
include $(CLEAR_VARS)
LOCAL_MODULE    := avcodec    #指定一个当前模块名1
LOCAL_SRC_FILES :=$(LOCAL_PATH)/../../../../media/ffmpeg/arm64-v8a/lib/libavcodec.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avfilter    #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/arm64-v8a/lib/libavfilter.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avformat    #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/arm64-v8a/lib/libavformat.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avutil   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/arm64-v8a/lib/libavutil.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

#include $(CLEAR_VARS)
#LOCAL_MODULE    := postproc    #指定一个当前模块名1
#LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/arm64-v8a/lib/libpostproc.so  #要编译的源文件
#include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := swresample   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/arm64-v8a/lib/libswresample.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := swscale   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/arm64-v8a/lib/libswscale.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)
endif
ifeq ($(TARGET_ARCH_ABI), arm64-v8a)
include $(CLEAR_VARS)
LOCAL_MODULE    :=mediaffmpeg
LOCAL_C_INCLUDES :=$(LOCAL_PATH)/../../../../media/ffmpeg/arm64-v8a/include/
LOCAL_SRC_FILES := nativeFfmpeg.cpp FFmpegEncode.cpp Util.cpp MediaCallBack.cpp FFmpegEncodeStream.cpp
LOCAL_SHARED_LIBRARIES := avformat avcodec avfilter avutil swresample swscale
LOCAL_LDLIBS    += -llog -landroid -lc
include $(BUILD_SHARED_LIBRARY)
endif
ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
include $(CLEAR_VARS)
LOCAL_MODULE    :=mediaffmpeg
LOCAL_C_INCLUDES :=$(LOCAL_PATH)/../../../../media/ffmpeg/armeabi-v7a/include/
LOCAL_SRC_FILES := nativeFfmpeg.cpp FFmpegEncode.cpp Util.cpp MediaCallBack.cpp FFmpegEncodeStream.cpp $(LOCAL_PATH)/videoRender/VideoRender.cpp

LOCAL_SHARED_LIBRARIES := avformat avcodec avfilter avutil swresample swscale
LOCAL_LDLIBS    += -llog -landroid -lc

include $(BUILD_SHARED_LIBRARY)
endif


