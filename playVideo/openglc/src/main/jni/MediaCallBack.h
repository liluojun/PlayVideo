//
// Created by Administrator on 2020/3/25.
//

#ifndef PLAYVIDEO_MEDIACALLBACK_H
#define PLAYVIDEO_MEDIACALLBACK_H


#include <stdint.h>
#include <jni.h>
#include "libavutil/pixfmt.h"
#include "libavutil/frame.h"

class MediaCallBack {
public:
    void CallBackDecodeData( AVFrame *mAVFrame);
    void CallBackDecodeDataAudio( uint8_t* data,int size);

public:
    JNIEnv *env;
    JavaVM *jvm;
    jobject returnBack;
//    jbyteArray y;
//    jbyteArray u;
//    jbyteArray v;
};


#endif //PLAYVIDEO_MEDIACALLBACK_H
