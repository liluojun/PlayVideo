//
// Created by Administrator on 2020/3/25.
//

#ifndef PLAYVIDEO_MEDIACALLBACK_H
#define PLAYVIDEO_MEDIACALLBACK_H


#include <stdint.h>
#include <jni.h>
#include "../../../../media/ffmpeg/include/libavutil/pixfmt.h"
#include "../../../../media/ffmpeg/include/libavutil/frame.h"

class MediaCallBack {
public:
    void CallBackDecodeData(AVFrame *mAVFrame);

    bool checkDataIsError(int w, int h);

public:
    JNIEnv *env;
    JavaVM *jvm;
    jobject returnBack;
    jbyteArray y;
    jbyteArray u;
    jbyteArray v;
};


#endif //PLAYVIDEO_MEDIACALLBACK_H
