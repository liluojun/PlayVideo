//
// Created by Administrator on 2020/3/26.
//

#include <jni.h>
#include <stddef.h>
#include <malloc.h>
#include <string.h>

static void initClassHelper(JNIEnv *env, const char *path, jobject *objptr) {
    jclass cls = env->FindClass(path);
    if (!cls) {
        return;
    }
    jmethodID constr = env->GetMethodID(cls, "<init>", "()V");
    if (!constr) {
        return;
    }
    jobject obj = env->NewObject(cls, constr);
    if (!obj) {
        return;
    }
    (*objptr) = env->NewGlobalRef(obj);
}

/*jstring 转char* */
static char* jstringTostr(JNIEnv* env, jstring jstr) {
    char* pStr = NULL;
    jclass jstrObj = env->FindClass("java/lang/String");
    jstring encode = env->NewStringUTF("utf-8");
    jmethodID methodId = env->GetMethodID(jstrObj, "getBytes",
                                          "(Ljava/lang/String;)[B");
    jbyteArray byteArray = (jbyteArray) env->CallObjectMethod(jstr, methodId,
                                                              encode);
    jsize strLen = env->GetArrayLength(byteArray);
    jbyte *jBuf = env->GetByteArrayElements(byteArray, JNI_FALSE);
    if (jBuf > 0) {
        pStr = (char*) malloc(strLen + 1);
        if (!pStr) {
            return NULL;
        }
        memcpy(pStr, jBuf, strLen);
        pStr[strLen] = 0;
    }

    env->ReleaseByteArrayElements(byteArray, jBuf, 0);
    env->DeleteLocalRef(byteArray);
    return pStr;
}
/*jbyteArrAy 转char* */
static unsigned char* jbyteArrayTochar(JNIEnv* env, jbyteArray jbyteArr) {
    jbyte* jbyteArr1 = (jbyte*) env->GetByteArrayElements(jbyteArr, 0);
    unsigned char* buf = (unsigned char*) jbyteArr1;
    return buf;
}