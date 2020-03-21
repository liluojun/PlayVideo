LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := avcodec    #指定一个当前模块名1
LOCAL_SRC_FILES :=$(LOCAL_PATH)/../../../../media/ffmpeg/lib/libavcodec.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avdevice    #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/lib/libavdevice.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avfilter    #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/lib/libavfilter.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avformat    #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/lib/libavformat.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avutil   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/lib/libavutil.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := postproc   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/lib/libpostproc.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := swresample   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/lib/libswresample.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := swscale   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/lib/libswscale.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE    :=mediaffmpeg
LOCAL_C_INCLUDES :=$(LOCAL_PATH)/../../../../media/ffmpeg/include/
LOCAL_SRC_FILES := nativeFfmpeg.cpp FFmpegEncode.cpp


LOCAL_SHARED_LIBRARIES := avformat avdevice avcodec  avfilter  avutil postproc swresample swscale

LOCAL_LDLIBS    += -llog -landroid -lc

include $(BUILD_SHARED_LIBRARY)



