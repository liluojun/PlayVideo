
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",__VA_ARGS__)

#include "com_media_opengl_OpenglSheard.h"
#include "FFmpegEncode.h"
#include "FFmpegEncodeStream.h"
#include "Util.cpp"

#ifdef __cplusplus
extern "C" {
#endif
#define MEDIAC_ALLBACK "com/media/opengl/OpenglSheard"
FFmpegEncode *encode = NULL;
FFmpegEncodeStream *encodeStream = NULL;
JavaVM *jvm;
MediaCallBack *mMediaCallBack;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;
    JNIEnv *env = NULL;
    jint result = -1;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return result;
    }
    return JNI_VERSION_1_6;
}

void releaseByteArray(JNIEnv *env, jbyteArray array, uint8_t *elems, jint mode) {
    env->ReleaseByteArrayElements(array, (jbyte *) elems, mode);
    env->DeleteLocalRef(array);
}

JNIEXPORT jint JNICALL Java_com_media_opengl_OpenglSheard_creatFfmpeg
        (JNIEnv *env, jobject obj) {
    encode = new FFmpegEncode();
    return encode->creatFFmpeg(AV_CODEC_ID_H264);
} ;
/*
 * Class:     sc_playvideo_videonative_MediaNative
 * Method:    initContext
 * Signature: (III)I
 */
JNIEXPORT jint JNICALL Java_com_media_opengl_OpenglSheard_initContext
        (JNIEnv *env, jobject obj, jint w, jint h, jint fps) {
    if (encode != NULL) {
        return encode->initAVCodecContext(w, h, fps, AV_PIX_FMT_NV21);
    } else {
        return -1;
    }
} ;

/*
 * Class:     sc_playvideo_videonative_MediaNative
 * Method:    encode
 * Signature: ([BI[B[B[BII)I
 */
JNIEXPORT jint JNICALL Java_com_media_opengl_OpenglSheard_encode
        (JNIEnv *env, jobject obj, jbyteArray src, jbyteArray y, jbyteArray v,
         jbyteArray u, jint w, jint h) {
    int result = -1;
    uint8_t *srcArray = (uint8_t *) env->GetByteArrayElements(src, 0);
    int lenth = env->GetArrayLength(src);
    uint8_t *yArray = (uint8_t *) env->GetByteArrayElements(y, 0);
    uint8_t *vArray = (uint8_t *) env->GetByteArrayElements(v, 0);
    uint8_t *uArray = (uint8_t *) env->GetByteArrayElements(u, 0);
    if (encode != NULL) {
        result = encode->encodeFFmpeg(srcArray, lenth, yArray, uArray, vArray, &w, &h,
                                      AV_PIX_FMT_NV21);
    }
    releaseByteArray(env, src, srcArray, 0);
    releaseByteArray(env, y, yArray, 0);
    releaseByteArray(env, v, vArray, 0);
    releaseByteArray(env, u, uArray, 0);
    return result;
} ;

JNIEXPORT void JNICALL Java_com_media_opengl_OpenglSheard_unFfmpeg
        (JNIEnv *env, jobject obj) {
    if (encode != NULL) {
        encode->unEncode();
        free(encode);
    }
}
/*
 * Class:     media_jni_MediaNative
 * Method:    openStream
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_media_opengl_OpenglSheard_openStream
        (JNIEnv *env, jobject obj, jstring path) {
    encodeStream = new FFmpegEncodeStream();
    mMediaCallBack = new MediaCallBack();
    mMediaCallBack->jvm = jvm;
    mMediaCallBack->env = env;
    encodeStream->mMediaCallBack = mMediaCallBack;
    initClassHelper(env, MEDIAC_ALLBACK, &(mMediaCallBack->returnBack));
    return encodeStream->openStream(jstringTostr(env, path));
} ;
/*
 * Class:     media_jni_MediaNative
 * Method:    closeStream
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_media_opengl_OpenglSheard_closeStream
        (JNIEnv *env, jobject obj) {

    if (encodeStream != NULL) {
        delete encodeStream;
    }
    if (mMediaCallBack != NULL) {
        delete mMediaCallBack;
    }
    if (encode != NULL) {
        encode->unEncode();
        delete encode;
    }
} ;
#ifdef __cplusplus
}
#endif
