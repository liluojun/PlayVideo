//
// Created by Administrator on 2019/10/20.
//

#ifndef PLAYVIDEO_FFMPEGENCOD_H
#define PLAYVIDEO_FFMPEGENCOD_H

#include "media_jni_MediaNative.h"
#include "GlThread.h"
#include "YuvData.h"
#ifdef __cplusplus
extern "C" {
#endif

class FFmpegEncode {
public:
    int creatFFmpeg(AVCodecID id);

    int encodeFFmpeg(uint8_t *framedata, int framelen,//input
                     uint8_t *outputY, uint8_t *outputU, uint8_t *outputV, int *width, int *height,
                     AVPixelFormat pixfmt);

    int initAVCodecContext(int width, int heigth, int fps, AVPixelFormat pixFmt);

    void unEncode();

    int renderToC(int width, int heigth,YuvData* yuvData);

public:
    AVCodec *mAVCodec = NULL;
    AVCodecContext *mAVCodecContext = NULL;
    AVFrame *mAVFrame = NULL;
    AVPacket *mAVPacket = NULL;
    GlThread *mGlThread = NULL;

};
#ifdef __cplusplus
}
#endif

#endif //PLAYVIDEO_FFMPEGENCOD_H
