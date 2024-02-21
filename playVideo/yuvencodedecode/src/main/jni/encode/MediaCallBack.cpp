//
// Created by Administrator on 2020/3/25.
//

#include <string.h>
#include "MediaCallBack.h"
#include <android/log.h>

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",__VA_ARGS__)

void MediaCallBack::CallBackDecodeData(AVFrame *mAVFrame) {
    JNIEnv *JniEnv;
    jvm->AttachCurrentThread(&JniEnv, NULL);
    jclass clazz = JniEnv->GetObjectClass(returnBack);
    if (clazz == NULL) {
        return;
    }
    jmethodID mid_static_method = JniEnv->GetStaticMethodID(clazz, "videoDataCallBackForJni",
                                                            "([B[B[BIII)V");
    if (mid_static_method == NULL) {
        JniEnv->DeleteLocalRef(clazz);
        return;
    }
    int line = mAVFrame->linesize[0];
    int w = mAVFrame->linesize[0];
    int h = mAVFrame->height;
    jbyteArray y = JniEnv->NewByteArray(w * h);
    jbyteArray u = JniEnv->NewByteArray(w * h / 4);
    jbyteArray v = JniEnv->NewByteArray(w * h / 4);
    JniEnv->SetByteArrayRegion(y, 0, w * h,
                               (jbyte *) mAVFrame->data[0]);
    JniEnv->SetByteArrayRegion(u, 0, w * h / 4,
                               (jbyte *) mAVFrame->data[1]);
    JniEnv->SetByteArrayRegion(v, 0, w * h / 4,
                               (jbyte *) mAVFrame->data[2]);
    JniEnv->CallStaticVoidMethod(clazz, mid_static_method, y, u, v, w,
                                 h, line);
    JniEnv->ReleaseByteArrayElements(y, JniEnv->GetByteArrayElements(y, 0), JNI_FALSE);
    JniEnv->DeleteLocalRef(y);
    JniEnv->ReleaseByteArrayElements(u, JniEnv->GetByteArrayElements(u, 0), JNI_FALSE);
    JniEnv->DeleteLocalRef(u);
    JniEnv->ReleaseByteArrayElements(v, JniEnv->GetByteArrayElements(v, 0), JNI_FALSE);
    JniEnv->DeleteLocalRef(v);
    JniEnv->DeleteLocalRef(clazz);
    JniEnv = NULL;
    jvm->DetachCurrentThread();
}

void MediaCallBack::CallBackDecodeDataAudio(uint8_t *data, int size) {
    JNIEnv *JniEnv;
    jvm->AttachCurrentThread(&JniEnv, NULL);
    jclass clazz = JniEnv->GetObjectClass(returnBack);
    if (clazz == NULL) {
        return;
    }
    jmethodID mid_static_method = JniEnv->GetStaticMethodID(clazz, "audioDataCallBackForJni",
                                                            "([B)V");
    if (mid_static_method == NULL) {
        JniEnv->DeleteLocalRef(clazz);
        return;
    }
    jbyteArray y = JniEnv->NewByteArray(size);
    JniEnv->SetByteArrayRegion(y, 0, size, (jbyte *) data);
    JniEnv->CallStaticVoidMethod(clazz, mid_static_method, y);
    JniEnv->ReleaseByteArrayElements(y, JniEnv->GetByteArrayElements(y, 0), JNI_FALSE);
    JniEnv->DeleteLocalRef(y);
    JniEnv->DeleteLocalRef(clazz);
    JniEnv = NULL;
    jvm->DetachCurrentThread();
}

