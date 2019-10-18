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
//    jbyteArray transitJbyteArray = env->NewByteArray(w * h * 3 / 2);
//    uint8_t *transitArray = (uint8_t *) env->GetByteArrayElements(transitJbyteArray, 0);
    jint uvW = w >> 1;
    jint ySize = w * h;
    jint uSize = (uvW) * (h >> 1);
    LOGE("uvw=%d  ysize=%d  usize=%d", uvW, ySize, uSize);
    uint8_t *y_src = srcArray;
    uint8_t *uv_src = srcArray + ySize;
//    uint8_t *y_tran = transitArray;
//    uint8_t *u_tran = transitArray + ySize;
//    uint8_t *v_tran = transitArray + ySize + uSize;
    uint8_t *y_dst = dstArray;
    uint8_t *u_dst = dstArray + ySize;
    uint8_t *v_dst = dstArray + ySize + uSize;
    libyuv::NV21ToI420(y_src, w, uv_src, w, y_dst, w, u_dst, uvW, v_dst, uvW, w, h);
//    libyuv::I420Rotate(y_tran, w, u_tran, w >> 1, v_tran, w >> 1, y_dst, h, u_dst, h >> 1, v_dst,
//                       h >> 1, h, w, libyuv::kRotate270);

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
    jint uvSize = (w >> 1) * (h >> 1);
    uint8_t *y_src = srcArray;
    uint8_t *uv_src = srcArray + ySize;
    uint8_t *u_src = srcArray + ySize;
    uint8_t *v_src = srcArray + ySize + uvSize;
    uint8_t *y_transit = transitArray;

    uint8_t *u_transit = transitArray + ySize;
    uint8_t *v_transit = transitArray + ySize + uvSize;
    uint8_t *y_dst = dstArray;
    uint8_t *uv_dst = dstArray + ySize;
    libyuv::NV21ToNV12((const uint8_t *) y_src, w, (const uint8_t *) uv_src, w, y_dst, w,
                       uv_dst, w,
                       w, h);
//    libyuv::NV12ToI420Rotate(transitArray, w, uv_transit, w, y_src, h, u_src, h >> 1, v_src, h >> 1,
//                             w, h, libyuv::kRotate270);
//    libyuv::NV21ToI420(y_src, w, uv_src, w, y_transit, w, u_transit, w >> 1, v_transit, w >> 1, w,
//                       h);
//    libyuv::I420Rotate(y_transit, w, u_transit, w >> 1, v_transit, w >> 1, y_src, h, u_src, h >> 1,
//                       v_src, h >> 1, w, h, libyuv::kRotate270);
//    libyuv::I420ToNV12(y_src, w, u_src, w >> 1, v_src, w >> 1, y_dst, w, uv_dst, w, w, h);
    env->ReleaseByteArrayElements(transitJbyteArray, (jbyte *) transitArray, 0);
    env->DeleteLocalRef(transitJbyteArray);
};
/*
 * Class:     media_jni_NativeMethod
 * Method:    nv21CutterToI420
 * Signature: ([B[BIIII)V
 */
JNIEXPORT void JNICALL Java_media_jni_NativeMethod_nv21CutterToI420
        (JNIEnv *env, jobject job, jbyteArray src, jbyteArray dst, jint cw, jint ch, jint w,
         jint h) {
    uint8_t *srcArray = (uint8_t *) env->GetByteArrayElements(src, 0);
    uint8_t *dstArray = (uint8_t *) env->GetByteArrayElements(dst, 0);
    jbyteArray transitJbyteArray = env->NewByteArray(w * h * 3 / 2);
    uint8_t *transitArray = (uint8_t *) env->GetByteArrayElements(transitJbyteArray, 0);

    jint ySize = w * h;
    jint startx = (w - cw) / 2;
    jint starty = (h - ch) / 2;
    jint uvSize = (w >> 1) * (h >> 1);
    uint8_t *y_src = srcArray;
    uint8_t *uv_src = srcArray + ySize;
    uint8_t *y_transit = transitArray;
    uint8_t *u_transit = transitArray + ySize;
    uint8_t *v_transit = transitArray + ySize + uvSize;
    uint8_t *y_dst = dstArray;
    uint8_t *u_dst = dstArray + (cw * ch);
    uint8_t *v_dst = dstArray + (cw * ch) + ((cw * ch) >> 2);
    libyuv::NV21ToI420(y_src, w, uv_src, w, y_transit, w, u_transit, w >> 1, v_transit, w >> 1, w,
                       h);
    libyuv::ConvertToI420(transitArray, ySize + uvSize, y_dst, cw, u_dst, cw >> 1,
                          v_dst, cw >> 1,
                          startx, starty, w, h, cw, ch, libyuv::kRotate0, libyuv::FOURCC_I420);
    env->ReleaseByteArrayElements(transitJbyteArray, (jbyte *) transitArray, 0);
    env->DeleteLocalRef(transitJbyteArray);

};

/*
 * Class:     media_jni_NativeMethod
 * Method:    nv21CutterToNv12
 * Signature: ([B[BIIII)V
 */
JNIEXPORT void JNICALL Java_media_jni_NativeMethod_nv21CutterToNv12
        (JNIEnv *env, jobject job, jbyteArray src, jbyteArray dst, jint cw, jint ch, jint w,
         jint h) {
    uint8_t *srcArray = (uint8_t *) env->GetByteArrayElements(src, 0);
    uint8_t *dstArray = (uint8_t *) env->GetByteArrayElements(dst, 0);
    jbyteArray transitJbyteArray = env->NewByteArray(w * h * 3 / 2);
    uint8_t *transitArray = (uint8_t *) env->GetByteArrayElements(transitJbyteArray, 0);
    jbyteArray transitCutJbyteArray = env->NewByteArray(cw * ch * 3 / 2);
    uint8_t *transitCutArray = (uint8_t *) env->GetByteArrayElements(transitCutJbyteArray, 0);
    jint ySize = w * h;
    jint startx = (w - cw) / 2;
    jint starty = (h - ch) / 2;
    jint uvSize = (w >> 1) * (h >> 1);
    uint8_t *y_src = srcArray;
    uint8_t *uv_src = srcArray + ySize;
    uint8_t *y_transit = transitArray;
    uint8_t *u_transit = transitArray + ySize;
    uint8_t *v_transit = transitArray + ySize + uvSize;
    uint8_t *y_dst = dstArray;
    uint8_t *uv_dst = dstArray + (cw * ch);
    //uint8_t *v_dst = dstArray + (cw * ch) + ((cw * ch) >> 2);
    uint8_t *y_transitCut = transitCutArray;
    uint8_t *u_transitCut = transitCutArray + (cw * ch);
    uint8_t *v_transitCut = transitCutArray + (cw * ch) + ((cw * ch) >> 2);

    libyuv::NV21ToI420(y_src, w, uv_src, w, y_transit, w, u_transit, w >> 1, v_transit, w >> 1, w,
                       h);
    libyuv::ConvertToI420(transitArray, ySize + uvSize, y_transitCut, cw, u_transitCut, cw >> 1,
                          v_transitCut, cw >> 1,
                          startx, starty, w, h, cw, ch, libyuv::kRotate0, libyuv::FOURCC_I420);
    libyuv::I420ToNV12(y_transitCut, cw, u_transitCut, cw >> 1, v_transitCut, cw >> 1, y_dst, cw,
                       uv_dst, cw, cw, ch);

    env->ReleaseByteArrayElements(transitJbyteArray, (jbyte *) transitArray, 0);
    env->DeleteLocalRef(transitJbyteArray);
    env->ReleaseByteArrayElements(transitCutJbyteArray, (jbyte *) transitCutArray, 0);
    env->DeleteLocalRef(transitCutJbyteArray);

};

