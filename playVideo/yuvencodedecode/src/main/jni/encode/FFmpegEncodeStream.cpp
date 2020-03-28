//
// Created by Administrator on 2020/3/21.
//

#include "FFmpegEncodeStream.h"
#include <pthread.h>

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"FFmpegEncodeStream",__VA_ARGS__)
typedef struct {
    AVCodec *mAVCodec;
    AVFrame *mAVFrame;
    AVPacket *mAVPacket;
    AVFormatContext *mAVformat;
    AVCodecContext *mAVCodecCtx;
    MediaCallBack *mMediaCallBack;
    int videoIndex;
    char *path;
} decodeContext;
char *videoPath = NULL;
MediaCallBack *mmMediaCallBack = NULL;
pthread_t pthread_ts[9]={-1L};
bool close_thread = true;

void *decodeStream(void *arg) {
    LOGE("decodeStream1");
    decodeContext context = {NULL, NULL, NULL, NULL, NULL, mmMediaCallBack, 0, NULL};
    decodeContext *mDecodeContext = &context;
    if (videoPath != NULL) {
        LOGE("decodeStream9 %s", videoPath);
        mDecodeContext->path = videoPath;
    } else {
        return 0;
    }
    avcodec_register_all();
    avformat_network_init();
    mDecodeContext->mAVformat = avformat_alloc_context();
    if (mDecodeContext->mAVformat == NULL) {
        return 0;
    }
    int result = avformat_open_input(&(mDecodeContext->mAVformat), mDecodeContext->path, NULL,
                                     NULL);
    if (result != 0) {
        LOGE("decodeStream10  %s", mDecodeContext->path);
        return 0;
    }

    result = avformat_find_stream_info(mDecodeContext->mAVformat, NULL);
    if (result < 0) {
        LOGE("decodeStream11");
        return 0;
    }
    if (mDecodeContext->mAVformat == NULL) {
        LOGE("mDecodeContext->mAVformat");
        return 0;
    }
    if (mDecodeContext->mAVformat->nb_streams == NULL) {
        LOGE("mDecodeContext->mAVformat->nb_streams=");
        return 0;
    }
    LOGE("mDecodeContext->mAVformat->nb_streams");
    LOGE("mDecodeContext->mAVformat->nb_streams=%d", mDecodeContext->mAVformat->nb_streams);
    for (int i = 0; i < mDecodeContext->mAVformat->nb_streams; i++) {
        if (mDecodeContext->mAVformat->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            mDecodeContext->videoIndex = i;
            break;
        }
    }
    AVCodecParameters *avCodecParameters = mDecodeContext->mAVformat->streams[mDecodeContext->videoIndex]->codecpar;
    mDecodeContext->mAVCodecCtx = avcodec_alloc_context3(NULL);
    if (mDecodeContext->mAVCodecCtx == NULL) {
        LOGE("alloc avcodec fail.");
        return 0;
    }
    if (avcodec_parameters_to_context(mDecodeContext->mAVCodecCtx, avCodecParameters) < 0) {
        LOGE("avcodec_parameters_to_context fail.");
    }
    LOGE("video %d*%d", mDecodeContext->mAVCodecCtx->width, mDecodeContext->mAVCodecCtx->height);
    mDecodeContext->mAVCodec = avcodec_find_decoder(mDecodeContext->mAVCodecCtx->codec_id);
    if (mDecodeContext->mAVCodec == NULL) {
        LOGE("alloc mAVCodec fail.%d", mDecodeContext->mAVCodecCtx->codec_id);
        return 0;
    }
    if (avcodec_open2(mDecodeContext->mAVCodecCtx, mDecodeContext->mAVCodec, NULL) < 0) {
        printf("avcodec_open2 error\n");
        return 0;
    }
    mDecodeContext->mAVFrame = av_frame_alloc();
    if (!mDecodeContext->mAVFrame) {
        printf("av_frame_alloc error\n");
        return 0;
    }

    mDecodeContext->mAVPacket = av_packet_alloc();
    if (mDecodeContext->mAVPacket == NULL) {
        printf("av_packet_alloc error\n");
        return 0;
    }
    while (close_thread) {
        if (av_read_frame(mDecodeContext->mAVformat, mDecodeContext->mAVPacket) >= 0 &&
            close_thread) {
            if (mDecodeContext->mAVPacket->stream_index == mDecodeContext->videoIndex &&
                close_thread) {
                int ret = avcodec_send_packet(mDecodeContext->mAVCodecCtx,
                                              mDecodeContext->mAVPacket);
                if (ret < 0) {
                    LOGE("video Error sending a packet for decoding.");
                    break;
                }
                while (ret >= 0 && close_thread) {
                    ret = avcodec_receive_frame(mDecodeContext->mAVCodecCtx,
                                                mDecodeContext->mAVFrame);
                    LOGE("decodeStream4.1 ret=%d", ret);
                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                        break;
                    } else if (ret < 0) {
                        LOGE("Error during decoding\n");
                        break;
                    }
                    if (mDecodeContext->mMediaCallBack != NULL) {
                        mDecodeContext->mMediaCallBack->CallBackDecodeData(
                                mDecodeContext->mAVFrame);
                    }
                }
            }
            av_packet_unref(mDecodeContext->mAVPacket);
        }
    }
    av_frame_free(&mDecodeContext->mAVFrame);
    avcodec_close(mDecodeContext->mAVCodecCtx);
    avformat_close_input(&mDecodeContext->mAVformat);
}

int FFmpegEncodeStream::openStream(char *path) {
    pthread_t id;
    videoPath = (char *) malloc(strlen(path));
    stpcpy(videoPath, path);
    mmMediaCallBack = mMediaCallBack;
    int err = pthread_create(&id, NULL, decodeStream, NULL);
    if (err != 0) {
        LOGE("thread error");
        return -1;
    }
    for (int i = 0; i < sizeof(pthread_ts) / sizeof(pthread_ts[0]); i++) {
        if (pthread_ts[i] == -1) {
            pthread_ts[i] = id;
        }
    }
}

FFmpegEncodeStream::~FFmpegEncodeStream() {
    close_thread = false;
    delete (videoPath);
    delete (mmMediaCallBack);
    for (int i = 0; i < sizeof(pthread_ts) / sizeof(pthread_ts[0]); i++) {
        if (pthread_ts[i] != -1) {
            int result = pthread_join(pthread_ts[i], NULL);
        }
    }
}
