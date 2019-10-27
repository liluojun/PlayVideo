//
// Created by Administrator on 2019/10/20.
//

#include "FfmpegEncod.h"

#ifdef __cplusplus
extern "C" {
#endif
int FfmpegEncod::creatFFmpeg() {
    avcodec_register_all();
    mAVCodec = avcodec_find_decoder(AVCodecID::AV_CODEC_ID_H264);
    if (mAVCodec == NULL) {
        printf("mAVCodec==NULL\n");
        return -1;
    }

    mAVCodecContext = avcodec_alloc_context3(mAVCodec);
    if (mAVCodecContext == NULL) {
        printf("mAVCodecContext==NULL\n");
        return -1;
    }
    if (avcodec_open2(mAVCodecContext, mAVCodec, NULL) < 0) {
        printf("avcodec_open2 error\n");
        return -1;
    }
    mAVFrame = av_frame_alloc();
    if (!mAVFrame) {
        printf("av_frame_alloc error\n");
        return -1;
    }
    mAVPacket = av_packet_alloc();
    if (!mAVPacket) {
        printf("av_packet_alloc error\n");
        return -1;
    }
    av_init_packet(mAVPacket);
    return 0;
}
int FfmpegEncod::initAVCodecContext(int width, int heigth, int fps, AVPixelFormat pixFmt) {
    if (mAVCodecContext == NULL) {
        printf("initAVCodecContext NULL\n");
        return -1;
    }
    if (width <= 0 || heigth <= 0 || fps <= 10) {
        printf("initAVCodecContext Parameters of the abnormal\n");
        return -1;
    }
    mAVCodecContext->width = width;
    mAVCodecContext->height = heigth;
    mAVCodecContext->frame_bits = fps;
    mAVCodecContext->pix_fmt = pixFmt;
}
int FfmpegEncod::encodeFFmpeg(uint8_t *framedata, int framelen,//input
                              uint8_t *outputY, uint8_t *outputU, uint8_t *outputV, int *width,
                              int *height,
                              AVPixelFormat *pixfmt) {
    if (mAVPacket == NULL) {
        printf("encodeFFmpeg NULL\n");
        return -1;
    }
    mAVPacket->data = framedata;
    mAVPacket->size = framelen;
    while (mAVPacket->size > 0) {
        if (avcodec_send_packet(mAVCodecContext, mAVPacket)) {
            printf("%s %d avcodec_send_packet fail\n", __func__, __LINE__);
            return -1;
        }
        if (avcodec_receive_frame(mAVCodecContext, mAVFrame)) {
            printf("%s %d avcodec_receive_frame fail\n", __func__, __LINE__);
            return -1;
        }
        switch (mAVCodecContext->pix_fmt) {
            case AV_PIX_FMT_YUV420P: {
                outputY = mAVFrame->data[0];
                outputU = mAVFrame->data[1];
                outputV = mAVFrame->data[2];
                width = &(mAVCodecContext->width);
                height = &(mAVCodecContext->height);
                pixfmt = &(mAVCodecContext->pix_fmt);
                break;
            }
            default: {
                printf("mAVCodecContext->pix_fmt Failure to identify\n");
                return -1;
            }
        }
        return 0;
    }
}
#ifdef __cplusplus
}

void
FfmpegEncod::copyYUVData(AVCodecContext *mAVCodecContext, AVPacket *pPacket, uint8_t *outputframe) {

}

#endif

