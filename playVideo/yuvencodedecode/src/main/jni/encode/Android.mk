LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := avcodec    #指定一个当前模块名1
LOCAL_SRC_FILES :=$(LOCAL_PATH)/../../../../media/ffmpeg/$(TARGET_ARCH_ABI)/lib/libavcodec.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avfilter    #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/$(TARGET_ARCH_ABI)/lib/libavfilter.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avformat    #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/$(TARGET_ARCH_ABI)/lib/libavformat.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := avutil   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/$(TARGET_ARCH_ABI)/lib/libavutil.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := swresample   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/$(TARGET_ARCH_ABI)/lib/libswresample.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := swscale   #指定一个当前模块名1
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../../media/ffmpeg/$(TARGET_ARCH_ABI)/lib/libswscale.so  #要编译的源文件
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    :=mediaffmpeg
LOCAL_C_INCLUDES :=$(LOCAL_PATH)/../../../../media/ffmpeg/$(TARGET_ARCH_ABI)/include/
LOCAL_SRC_FILES := nativeFfmpeg.cpp FFmpegEncode.cpp Util.cpp MediaCallBack.cpp\
                   FFmpegEncodeStream.cpp VideoRender.cpp EglEnvironment.cpp EglUtils.cpp GlRendering.cpp\
                   GlDraw.cpp GlThread.cpp Looper.cpp

LOCAL_SHARED_LIBRARIES := avformat avcodec avfilter avutil swresample swscale
LOCAL_LDLIBS    += -llog -landroid -lc -lGLESv2 -lEGL
LOCAL_CPPFLAGS += -std=c++11
include $(BUILD_SHARED_LIBRARY)


