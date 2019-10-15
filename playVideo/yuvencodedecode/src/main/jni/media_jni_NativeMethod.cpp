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
    jint uvW = w >> 1;
    jint ySize = w * h;
    jint uSize = (uvW) * (h >> 1);
    LOGE("uvw=%d  ysize=%d  usize=%d", uvW, ySize, uSize);
    uint8_t *y_src = srcArray;
    uint8_t *uv_src = srcArray + ySize;
    uint8_t *y_dst = dstArray;
    uint8_t *u_dst = dstArray + ySize;
    uint8_t *v_dst = dstArray + ySize + uSize;
    libyuv::NV21ToI420(y_src, w, uv_src, w, y_dst, w, u_dst, uvW, v_dst, uvW, w, h);

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
    jbyteArray transitJbyteArray=env->NewByteArray(w*h*3/2);
    uint8_t *transitArray = (uint8_t *) env->GetByteArrayElements(transitJbyteArray, 0);
    jint ySize = w * h;

    LOGE("w=%d  h=%d  ysize=%d", w, h, ySize);
    uint8_t *y_src = srcArray;
    uint8_t *uv_src = srcArray + ySize;
    uint8_t *y_transit = transitArray;
    uint8_t *uv_transit = transitArray + ySize;
    uint8_t *y_dst = dstArray;
    uint8_t *u_dst = dstArray + ySize;
    uint8_t *v_dst = dstArray + ySize+(w>>1) * (h >> 1);
    libyuv::NV21ToNV12((const uint8_t *) y_src, w, (const uint8_t *) uv_src, w, y_transit, w, uv_transit, w,
                       w, h);
    libyuv::NV12ToI420Rotate(y_transit,w,uv_transit,w,y_dst,h,u_dst,h,v_dst,h,w,h,libyuv::kRotate270);
    env->ReleaseByteArrayElements(transitJbyteArray, (jbyte *)transitArray, 0);
    env->DeleteLocalRef(transitJbyteArray);
};
