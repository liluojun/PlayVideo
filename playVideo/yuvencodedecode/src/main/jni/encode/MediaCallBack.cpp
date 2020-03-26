//
// Created by Administrator on 2020/3/25.
//

#include <string.h>
#include "MediaCallBack.h"
#include <android/log.h>

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"FFmpegEncodeStream",__VA_ARGS__)

void MediaCallBack::CallBackDecodeData(AVFrame *mAVFrame) {
    JNIEnv *JniEnv = env;
    //jvm->AttachCurrentThread(&JniEnv, NULL);
    jclass clazz = JniEnv->GetObjectClass(returnBack);
    if (clazz == NULL) {
        return;
    }
    jmethodID mid_static_method = JniEnv->GetStaticMethodID(clazz, "videoDataCallBackForJni",
                                                      "([B[B[BII)V");
    if (mid_static_method == NULL) {
        JniEnv->DeleteLocalRef(clazz);
        return;
    }
    if (checkDataIsError(mAVFrame->width, mAVFrame->height)) {
        LOGE("00");
        y = env->NewByteArray(mAVFrame->width * mAVFrame->height);
        u = env->NewByteArray(mAVFrame->width * mAVFrame->height / 4);
        v = env->NewByteArray(mAVFrame->width * mAVFrame->height / 4);
    }
    LOGE("Ysize=%d ***yData=%d***%d", sizeof(uint8_t) * mAVFrame->width * mAVFrame->height,
         sizeof(*(mAVFrame->data[0])), mAVFrame->linesize);
    env->SetByteArrayRegion(y, 0, mAVFrame->width * mAVFrame->height, (jbyte *) mAVFrame->data[0]);
    LOGE("11");
    env->SetByteArrayRegion(u, 0, mAVFrame->width * mAVFrame->height / 4,
                            (jbyte *) mAVFrame->data[1]);
    LOGE("22");
    env->SetByteArrayRegion(v, 0, mAVFrame->width * mAVFrame->height / 4,
                            (jbyte *) mAVFrame->data[2]);
    LOGE("33");
    JniEnv->CallStaticVoidMethod(clazz, mid_static_method, y, u, v, mAVFrame->width, mAVFrame->height);
    LOGE("44");
    JniEnv->DeleteLocalRef(clazz);
    JniEnv = NULL;
    // jvm->DetachCurrentThread();
}

bool MediaCallBack::checkDataIsError(int w, int h) {
    if (y != NULL && v != NULL && u != NULL && env->GetArrayLength(y) == w * h && env->GetArrayLength(u) == w * h / 4 &&
            env->GetArrayLength(v)== w * h / 4) {
        return false;
    }
    return true;
}
