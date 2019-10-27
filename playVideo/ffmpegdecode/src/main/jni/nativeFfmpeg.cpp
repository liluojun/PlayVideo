//
// Created by Administrator on 2019/8/23.
//
#include <jni.h>
#include <android/log.h>
#include "sc_playvideo_videonative_MediaNative.h"
#include "FfmpegEncod.h"

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",__VA_ARGS__)
FfmpegEncod *encode;

JNIEXPORT jint JNICALL Java_sc_playvideo_videonative_MediaNative_creatFfmpeg
        (JNIEnv *env, jobject obj) {
    encode = new FfmpegEncod();
    return 0;
};