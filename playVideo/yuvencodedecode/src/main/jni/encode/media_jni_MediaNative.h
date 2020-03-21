/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class media_jni_MediaNative */

#ifndef _Included_media_jni_MediaNative
#define _Included_media_jni_MediaNative
#ifdef __cplusplus
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include <android/log.h>
#endif
/*
 * Class:     media_jni_MediaNative
 * Method:    creatFfmpeg
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_media_jni_MediaNative_creatFfmpeg
  (JNIEnv *, jobject);

/*
 * Class:     media_jni_MediaNative
 * Method:    initContext
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_media_jni_MediaNative_initContext
  (JNIEnv *, jobject, jint, jint, jint);

/*
 * Class:     media_jni_MediaNative
 * Method:    encode
 * Signature: ([B[B[B[BII)I
 */
JNIEXPORT jint JNICALL Java_media_jni_MediaNative_encode
  (JNIEnv *, jobject, jbyteArray, jbyteArray, jbyteArray, jbyteArray, jint, jint);

/*
 * Class:     media_jni_MediaNative
 * Method:    unFfmpeg
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_media_jni_MediaNative_unFfmpeg
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif