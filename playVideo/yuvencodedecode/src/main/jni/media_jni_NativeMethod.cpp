//
// Created by Administrator on 2019/10/15.
//
#include "libyuv.h"
#include <android/log.h>
#include "media_jni_NativeMethod.h"

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",__VA_ARGS__)
/*
 * Class:     media_jni_NativeMethod
 * Method:    nv21ToI420
 * Signature: ([B[BII)V
 */
JNIEXPORT void JNICALL Java_media_jni_NativeMethod_nv21ToI420
        (JNIEnv *env, jobject job, jbyteArray src, jbyteArray dst, jint w, jint h) {
    uint8_t *srcArray = (uint8_t *) env->GetByteArrayElements(src, NULL);
    uint8_t *dstArray = (uint8_t *) env->GetByteArrayElements(dst, NULL);
    jbyteArray transitJbyteArray = env->NewByteArray(w * h * 3 / 2);
    uint8_t *transitArray = (uint8_t *) env->GetByteArrayElements(transitJbyteArray, 0);
    jint uvW = w >> 1;
    jint ySize = w * h;
    jint uSize = (uvW) * (h >> 1);
    LOGE("uvw=%d  ysize=%d  usize=%d", uvW, ySize, uSize);
    uint8_t *y_src = srcArray;
    uint8_t *uv_src = srcArray + ySize;
    uint8_t *y_tran = transitArray;
    uint8_t *u_tran = transitArray + ySize;
    uint8_t *v_tran = transitArray + ySize + uSize;
    uint8_t *y_dst = dstArray;
    uint8_t *u_dst = dstArray + ySize;
    uint8_t *v_dst = dstArray + ySize + uSize;
    libyuv::NV21ToI420(y_src, w, uv_src, w, y_tran, w, u_tran, uvW, v_tran, uvW, w, h);
    libyuv::I420Rotate(y_tran, w, u_tran, w >> 1, v_tran, w >> 1, y_dst, h, u_dst, h >> 1, v_dst,
                       h >> 1, h, w, libyuv::kRotate270);
};

/*
 * Class:     media_jni_NativeMethod
 * Method:    nv21ToNv12
 * Signature: ([B[BII)V
 */
JNIEXPORT void JNICALL Java_media_jni_NativeMethod_nv21ToNv12
        (JNIEnv *env, jobject job, jbyteArray src, jbyteArray dst, jint w, jint h) {
    uint8_t *srcArray = (uint8_t *) env->GetByteArrayElements(src, 0);
    uint8_t *dstArray = (uint8_t *) env->GetByteArrayElements(dst, 0);
    jbyteArray transitJbyteArray = env->NewByteArray(w * h * 3 / 2);
    uint8_t *transitArray = (uint8_t *) env->GetByteArrayElements(transitJbyteArray, 0);
    jint ySize = w * h;

    LOGE("w=%d  h=%d  ysize=%d", w, h, ySize);
    uint8_t *y_src = srcArray;
    uint8_t *uv_src = srcArray + ySize;
    uint8_t *u_src = srcArray + ySize;
    uint8_t *v_src = srcArray + ySize + (w >> 1) * (h >> 1);
    uint8_t *y_transit = transitArray;
    uint8_t *uv_transit = transitArray + ySize;
    uint8_t *y_dst = dstArray;
    uint8_t *uv_dst = dstArray + ySize;
    libyuv::NV21ToNV12((const uint8_t *) y_src, w, (const uint8_t *) uv_src, w, y_transit, w,
                       uv_transit, w,
                       w, h);
    libyuv::NV12ToI420Rotate(transitArray, w, uv_transit, w, y_src, h, u_src, h >> 1, v_src, h >> 1,
                             w, h, libyuv::kRotate270);
    libyuv::I420ToNV12(y_src, h, u_src, h >> 1, v_src, h >> 1, y_dst, h, uv_dst, h, h, w);
    env->ReleaseByteArrayElements(transitJbyteArray, (jbyte *) transitArray, 0);
    env->DeleteLocalRef(transitJbyteArray);
};
