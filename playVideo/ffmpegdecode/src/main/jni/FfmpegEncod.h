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

    AVCodec *mAVCodec = NULL;
    AVCodecContext *mAVCodecContext=NULL;
};
#ifdef __cplusplus
}
#endif

#endif //PLAYVIDEO_FFMPEGENCOD_H
