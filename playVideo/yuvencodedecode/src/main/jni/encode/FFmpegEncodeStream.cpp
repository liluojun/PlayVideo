//
// Created by Administrator on 2020/3/21.
//

#include "FFmpegEncodeStream.h"

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"FFmpegEncodeStream",__VA_ARGS__)

int FFmpegEncodeStream::openStream(char *path) {
    if (*path == NULL) {
        return -1;
    }
    avcodec_register_all();
    avformat_network_init();
    mAVformat = avformat_alloc_context();
    int result = avformat_open_input(&mAVformat, path, NULL, NULL);
    if (result != 0) {
        return -2;
    }

    result = avformat_find_stream_info(mAVformat, NULL);
    if (result < 0) {
        return -3;
    }
    int videoIndex = -1;
    for (int i = 0; i < mAVformat->nb_streams; i++) {
        if (mAVformat->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }
    //mAVCodecCtx = mAVformat->streams[videoIndex]->codec;
    AVCodecParameters *avCodecParameters = mAVformat->streams[videoIndex]->codecpar;
    mAVCodecCtx = avcodec_alloc_context3(NULL);
    if (mAVCodecCtx == NULL) {
        LOGE("alloc avcodec fail.");
        return -1;
    }
    if (avcodec_parameters_to_context(mAVCodecCtx, avCodecParameters) < 0) {
        LOGE("avcodec_parameters_to_context fail.");
    }
    LOGE("video %d*%d", mAVCodecCtx->width, mAVCodecCtx->height);
    mAVCodec = avcodec_find_decoder(mAVCodecCtx->codec_id);
    if (mAVCodec == NULL) {
        LOGE("alloc mAVCodec fail.%d", mAVCodecCtx->codec_id);
        return -1;
    }
    if (avcodec_open2(mAVCodecCtx, mAVCodec, NULL) < 0) {
        printf("avcodec_open2 error\n");
        return -1;
    }
    mAVFrame = av_frame_alloc();
    if (!mAVFrame) {
        printf("av_frame_alloc error\n");
        return -1;
    }

    mAVPacket = av_packet_alloc();
    if (mAVPacket == NULL) {
        printf("av_packet_alloc error\n");
        return -1;
    }
    for (;;) {
        if (av_read_frame(mAVformat, mAVPacket) >= 0) {
            if (mAVPacket->stream_index == videoIndex) {
                int ret = avcodec_send_packet(mAVCodecCtx, mAVPacket);
                if (ret < 0) {
                    LOGE("video Error sending a packet for decoding.");
                    break;
                }
                while (ret >= 0) {
                    ret = avcodec_receive_frame(mAVCodecCtx, mAVFrame);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                        break;
                    } else if (ret < 0) {
                        LOGE("Error during decoding\n");
                        break;
                    }
                    if (mMediaCallBack != NULL) {
                        mMediaCallBack->CallBackDecodeData(mAVFrame);
                    }
                }
            }
            av_packet_unref(mAVPacket);
        }
    }
    av_frame_free(&mAVFrame);
    avcodec_close(mAVCodecCtx);
    avformat_close_input(&mAVformat);
}
