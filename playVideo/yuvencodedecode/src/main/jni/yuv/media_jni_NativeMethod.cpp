//
// Created by Administrator on 2019/10/15.
//

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",__VA_ARGS__)
#ifndef _Included_media_jni_NativeMethod
#define _Included_media_jni_NativeMethod
#ifdef __cplusplus
extern "C" {
#include "libyuv.h"
#include <android/log.h>
#include "media_jni_NativeMethod.h"
#include<string.h>
#endif
void releaseByteArray(JNIEnv *env, jbyteArray array, uint8_t *elems, jint mode) {
    env->ReleaseByteArrayElements(array, (jbyte *) elems, mode);
    env->DeleteLocalRef(array);
}
/*
 * Class:     media_jni_NativeMethod
 * Method:    nv21ToI420
 * Signature: ([B[BII)V
 */
JNIEXPORT void JNICALL Java_media_jni_NativeMethod_nv21ToI420
        (JNIEnv *env, jobject job, jbyteArray src, jbyteArray dst, jint w, jint h, jbyteArray y,
         jbyteArray u, jbyteArray v) {
    uint8_t *srcArray = (uint8_t *) env->GetByteArrayElements(src, NULL);
    uint8_t *dstArray = (uint8_t *) env->GetByteArrayElements(dst, NULL);
    uint8_t *yArray = (uint8_t *) env->GetByteArrayElements(y, NULL);
    uint8_t *uArray = (uint8_t *) env->GetByteArrayElements(u, NULL);
    uint8_t *vArray = (uint8_t *) env->GetByteArrayElements(v, NULL);
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
    memcpy(yArray, y_dst, ySize);
    memcpy(uArray, u_dst, (w * h) >> 2);
    memcpy(vArray, v_dst, (w * h) >> 2);
    //I420旋转方法注释但不能删除
//    yuv.libyuv::I420Rotate(y_tran, w, u_tran, w >> 1, v_tran, w >> 1, y_dst, h, u_dst, h >> 1, v_dst,
//                       h >> 1, h, w, yuv.libyuv::kRotate270);
    releaseByteArray(env, src, srcArray, 0);
    releaseByteArray(env, dst, dstArray, 0);
    releaseByteArray(env, y, yArray, 0);
    releaseByteArray(env, v, vArray, 0);
    releaseByteArray(env, u, uArray, 0);

} ;

/*
 * Class:     media_jni_NativeMethod
 * Method:    nv21ToNv12
 * Signature: ([B[BII)V
 */
JNIEXPORT void JNICALL Java_media_jni_NativeMethod_nv21ToNv12
        (JNIEnv *env, jobject job, jbyteArray src, jbyteArray dst, jint w, jint h, jbyteArray y,
         jbyteArray u, jbyteArray v) {

    uint8_t *srcArray = (uint8_t *) env->GetByteArrayElements(src, 0);
    uint8_t *dstArray = (uint8_t *) env->GetByteArrayElements(dst, 0);

    uint8_t *yArray = (uint8_t *) env->GetByteArrayElements(y, NULL);
    uint8_t *uArray = (uint8_t *) env->GetByteArrayElements(u, NULL);
    uint8_t *vArray = (uint8_t *) env->GetByteArrayElements(v, NULL);
//    jbyteArray transitJbyteArray = env->NewByteArray(w * h * 3 / 2);
//    uint8_t *transitArray = (uint8_t *) env->GetByteArrayElements(transitJbyteArray, 0);
    jint ySize = w * h;
    //LOGE("w=%d  h=%d  ysize=%d", w, h, ySize);
    jint uvSize = (w >> 1) * (h >> 1);
    uint8_t *y_src = srcArray;
    uint8_t *uv_src = srcArray + ySize;
 //   uint8_t *u_src = srcArray + ySize;
//    uint8_t *v_src = srcArray + ySize + uvSize;
//    uint8_t *y_transit = transitArray;
//    uint8_t *u_transit = transitArray + ySize;
//    uint8_t *v_transit = transitArray + ySize + uvSize;
    uint8_t *y_dst = dstArray;
    uint8_t *uv_dst = dstArray + ySize;

    libyuv::NV21ToI420(y_src, w, uv_src, w, yArray, w, uArray, w >> 1, vArray, w >> 1, w,
                       h);
    //I420旋转方法注释但不能删除
    //libyuv::I420Rotate(y_transit, w, u_transit, w >> 1, v_transit, w >> 1, y_src, h, u_src, h >> 1,
     //                  v_src, h >> 1, w, h, libyuv::kRotate270);
    libyuv::I420ToNV12(yArray, w, uArray, w >> 1, vArray, w >> 1, y_dst, w, uv_dst, w, w, h);
//    libyuv::NV21ToI420(y_src, w, uv_src, w, y_transit, w, u_transit, w >> 1, v_transit, w >> 1, w,
//                       h);
//    //I420旋转方法注释但不能删除
//    libyuv::I420Rotate(y_transit, w, u_transit, w >> 1, v_transit, w >> 1, yArray, h, uArray, h >> 1,
//                       vArray, h >> 1, w, h, libyuv::kRotate270);
//    libyuv::I420ToNV12(yArray, w, uArray, w >> 1, vArray, w >> 1, y_dst, w, uv_dst, w, w, h);
    releaseByteArray(env, src, srcArray, 0);
    releaseByteArray(env, dst, dstArray, 0);
    releaseByteArray(env, y, yArray, 0);
    releaseByteArray(env, v, vArray, 0);
    releaseByteArray(env, u, uArray, 0);
} ;
/*
 * Class:     media_jni_NativeMethod
 * Method:    nv21CutterToI420
 * Signature: ([B[BIIII)V
 */
JNIEXPORT void JNICALL Java_media_jni_NativeMethod_nv21CutterToI420
        (JNIEnv *env, jobject job, jbyteArray src, jbyteArray dst, jint cw, jint ch, jint w,
         jint h, jbyteArray y,
         jbyteArray u, jbyteArray v) {
    uint8_t *srcArray = (uint8_t *) env->GetByteArrayElements(src, 0);
    uint8_t *dstArray = (uint8_t *) env->GetByteArrayElements(dst, 0);
    jbyteArray transitJbyteArray = env->NewByteArray(w * h * 3 / 2);
    uint8_t *transitArray = (uint8_t *) env->GetByteArrayElements(transitJbyteArray, 0);
    uint8_t *yArray = (uint8_t *) env->GetByteArrayElements(y, NULL);
    uint8_t *uArray = (uint8_t *) env->GetByteArrayElements(u, NULL);
    uint8_t *vArray = (uint8_t *) env->GetByteArrayElements(v, NULL);
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
    memcpy(yArray, y_dst, cw * ch);
    memcpy(uArray, u_dst, (cw * ch) >> 2);
    memcpy(vArray, v_dst, (cw * ch) >> 2);
    releaseByteArray(env, transitJbyteArray, transitArray, 0);
    releaseByteArray(env, src, srcArray, 0);
    releaseByteArray(env, dst, dstArray, 0);
    releaseByteArray(env, y, yArray, 0);
    releaseByteArray(env, v, vArray, 0);
    releaseByteArray(env, u, uArray, 0);
} ;

/*
 * Class:     media_jni_NativeMethod
 * Method:    nv21CutterToNv12
 * Signature: ([B[BIIII)V
 */
JNIEXPORT void JNICALL Java_media_jni_NativeMethod_nv21CutterToNv12
        (JNIEnv *env, jobject job, jbyteArray src, jbyteArray dst, jint cw, jint ch, jint w,
         jint h, jbyteArray y,
         jbyteArray u, jbyteArray v) {
    uint8_t *srcArray = (uint8_t *) env->GetByteArrayElements(src, 0);
    uint8_t *dstArray = (uint8_t *) env->GetByteArrayElements(dst, 0);
    uint8_t *yArray = (uint8_t *) env->GetByteArrayElements(y, NULL);
    uint8_t *uArray = (uint8_t *) env->GetByteArrayElements(u, NULL);
    uint8_t *vArray = (uint8_t *) env->GetByteArrayElements(v, NULL);
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
    uint8_t *uv_dst = dstArray + (cw * ch);
    libyuv::NV21ToI420(y_src, w, uv_src, w, y_transit, w, u_transit, w >> 1, v_transit, w >> 1, w,
                       h);
    libyuv::ConvertToI420(transitArray, ySize + uvSize, yArray, cw, uArray, cw >> 1,
                          vArray, cw >> 1,
                          startx, starty, w, h, cw, ch, libyuv::kRotate0, libyuv::FOURCC_I420);

    libyuv::I420ToNV12(yArray, cw, uArray, cw >> 1, vArray, cw >> 1, y_dst, cw,
                       uv_dst, cw, cw, ch);
    releaseByteArray(env, transitJbyteArray, transitArray, 0);
    releaseByteArray(env, src, srcArray, 0);
    releaseByteArray(env, dst, dstArray, 0);
    releaseByteArray(env, y, yArray, 0);
    releaseByteArray(env, v, vArray, 0);
    releaseByteArray(env, u, uArray, 0);
} ;
#ifdef __cplusplus
}
#endif
#endif

