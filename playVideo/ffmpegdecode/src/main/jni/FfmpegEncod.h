//
// Created by Administrator on 2019/10/20.
//

#ifndef PLAYVIDEO_FFMPEGENCOD_H
#define PLAYVIDEO_FFMPEGENCOD_H

#ifdef __cplusplus
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#endif
class FfmpegEncod {
public:
    int creatFFmpeg();

    int encodeFFmpeg(uint8_t *framedata, int framelen,//input
                     uint8_t *outputY,uint8_t *outputU,uint8_t *outputV, int *width, int *height, AVPixelFormat *pixfmt);

    int initAVCodecContext(int width, int heigth, int fps, AVPixelFormat pixFmt);

    void copyYUVData(AVCodecContext *pPacket, AVPacket *outputframe, uint8_t *string);

public:
    AVCodec *mAVCodec = NULL;
    AVCodecContext *mAVCodecContext = NULL;
    AVFrame *mAVFrame = NULL;
    AVPacket *mAVPacket = NULL;
};
#ifdef __cplusplus
}
#endif

#endif //PLAYVIDEO_FFMPEGENCOD_H
