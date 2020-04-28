//
// Created by Administrator on 2020/3/21.
//

#include "FFmpegEncodeStream.h"
#include <pthread.h>

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"FFmpegEncodeStream",__VA_ARGS__)
#define MAX_AUDIO_FRAME_SIZE 44100
typedef struct {
    AVCodec *mAVCodec;
    AVFrame *mAVFrame;
    AVPacket *mAVPacket;
    AVFormatContext *mAVformat;
    AVCodecContext *mAVCodecCtx;
    MediaCallBack *mMediaCallBack;
    int videoIndex, audioIndex;
    char *path;
} decodeContext;
char *videoPath = NULL;
MediaCallBack *mmMediaCallBack = NULL;
pthread_t pthread_ts[9] = {-1L};
bool close_thread = true;

int decodeVideo() {
    decodeContext context = {NULL, NULL, NULL, NULL, NULL, mmMediaCallBack, -1, -1, NULL};
    decodeContext *mDecodeContext = &context;
    if (videoPath != NULL) {
        mDecodeContext->path = videoPath;
    } else {
        return 0;
    }
    mDecodeContext->mAVformat = avformat_alloc_context();
    if (mDecodeContext->mAVformat == NULL) {
        return 0;
    }
    int result = avformat_open_input(&(mDecodeContext->mAVformat), mDecodeContext->path, NULL,
                                     NULL);
    if (result != 0) {
        LOGE("avformat_open_input ERROR");
        return 0;
    }

    result = avformat_find_stream_info(mDecodeContext->mAVformat, NULL);
    if (result < 0) {
        LOGE("avformat_find_stream_info ERROR");
        return 0;
    }
    if (mDecodeContext->mAVformat == NULL) {
        LOGE("mDecodeContext->mAVformat  ERROR");
        return 0;
    }
    if (mDecodeContext->mAVformat->nb_streams == NULL) {
        LOGE("mDecodeContext->mAVformat->nb_streams  ERROR");
        return 0;
    }
    for (int i = 0; i < mDecodeContext->mAVformat->nb_streams; i++) {
        if (mDecodeContext->mAVformat->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            mDecodeContext->videoIndex = i;
            break;
        }
    }
    for (int i = 0; i < mDecodeContext->mAVformat->nb_streams; i++) {
        if (mDecodeContext->mAVformat->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            mDecodeContext->audioIndex = i;
            break;
        }
    }
    AVCodecParameters *avCodecParameters = mDecodeContext->mAVformat->streams[mDecodeContext->audioIndex]->codecpar;
    mDecodeContext->mAVCodecCtx = avcodec_alloc_context3(NULL);
    if (mDecodeContext->mAVCodecCtx == NULL) {
        LOGE("mDecodeContext->mAVCodecCtx  ERROR");
        return 0;
    }
    if (avcodec_parameters_to_context(mDecodeContext->mAVCodecCtx, avCodecParameters) < 0) {
    }
    mDecodeContext->mAVCodec = avcodec_find_decoder(mDecodeContext->mAVCodecCtx->codec_id);
    if (mDecodeContext->mAVCodec == NULL) {
        LOGE("mDecodeContext->mAVCodec  ERROR");
        return 0;
    }
    if (avcodec_open2(mDecodeContext->mAVCodecCtx, mDecodeContext->mAVCodec, NULL) < 0) {
        printf("avcodec_open2 error\n");
        LOGE("avcodec_open2  ERROR");
        return 0;
    }
    mDecodeContext->mAVFrame = av_frame_alloc();
    if (!mDecodeContext->mAVFrame) {
        printf("av_frame_alloc error\n");
        LOGE("av_frame_alloc  ERROR");
        return 0;
    }

    mDecodeContext->mAVPacket = av_packet_alloc();
    if (mDecodeContext->mAVPacket == NULL) {
        printf("av_packet_alloc error\n");
        LOGE("av_packet_alloc  ERROR");
        return 0;
    }
    SwrContext *swrCtx = swr_alloc();
    //重采样设置选项-----------------------------------------------------------start
    //输入的采样格式
    enum AVSampleFormat in_sample_fmt = mDecodeContext->mAVCodecCtx->sample_fmt;
    //输出的采样格式 16bit PCM
    enum AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
    //输入的采样率
    int in_sample_rate = mDecodeContext->mAVCodecCtx->sample_rate;
    //输出的采样率
    int out_sample_rate = 44100;
    //输入的声道布局
    uint64_t in_ch_layout = mDecodeContext->mAVCodecCtx->channel_layout;
    //输出的声道布局
    uint64_t out_ch_layout = AV_CH_LAYOUT_STEREO;

    swr_alloc_set_opts(swrCtx, out_ch_layout, out_sample_fmt, out_sample_rate, in_ch_layout,
                       in_sample_fmt,
                       in_sample_rate, 0, 0);
    swr_init(swrCtx);
    //存储pcm数据
    uint8_t *out_buffer ;
    av_samples_alloc(&out_buffer, NULL, 2, out_sample_rate,
                     AV_SAMPLE_FMT_S16, 0);
    //重采样设置选项-----------------------------------------------------------end
    int ret, got_frame, framecount = 0;
    while (close_thread) {
        if (av_read_frame(mDecodeContext->mAVformat, mDecodeContext->mAVPacket) >= 0 &&
            close_thread) {
            /*if (mDecodeContext->mAVPacket->stream_index == mDecodeContext->videoIndex &&
                close_thread) {
                int ret = avcodec_send_packet(mDecodeContext->mAVCodecCtx,
                                              mDecodeContext->mAVPacket);
                if (ret < 0) {
                    break;
                }
                while (ret >= 0 && close_thread) {
                    ret = avcodec_receive_frame(mDecodeContext->mAVCodecCtx,
                                                mDecodeContext->mAVFrame);

                    if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                        break;
                    } else if (ret < 0) {

                        break;
                    }
                    switch (mDecodeContext->mAVCodecCtx->codec_id) {
                        case AV_CODEC_ID_H265:
                        case AV_CODEC_ID_H264:
                            if (mDecodeContext->mMediaCallBack != NULL) {
                                mDecodeContext->mMediaCallBack->CallBackDecodeData(
                                        mDecodeContext->mAVFrame);
                            }
                            break;
                        case AV_CODEC_ID_MPEG4:
                            break;
                    }

                }
            } else*/ if (mDecodeContext->mAVPacket->stream_index == mDecodeContext->audioIndex &&
                         close_thread) {
                ret = avcodec_decode_audio4(mDecodeContext->mAVCodecCtx, mDecodeContext->mAVFrame,
                                            &got_frame, mDecodeContext->mAVPacket);
                if (ret < 0) {
                    printf("%s", "解码完成");
                }
                //非0，正在解码
                if (close_thread) {
                    //printf("解码%d帧", framecount++);
                    int64_t delay = swr_get_delay(swrCtx, mDecodeContext->mAVFrame->sample_rate);
                    int64_t out_count = av_rescale_rnd(
                            mDecodeContext->mAVFrame->nb_samples + delay, //本次要处理的数据个数
                            out_sample_rate,
                            mDecodeContext->mAVFrame->sample_rate,
                            AV_ROUND_UP);
                    LOGE("解码%d帧", framecount++);
                    int count=swr_convert(swrCtx, &out_buffer, out_count,
                                (const uint8_t **) mDecodeContext->mAVFrame->data,
                                mDecodeContext->mAVFrame->nb_samples);
                    int size = av_samples_get_buffer_size(NULL, 2, out_count,
                                                      out_sample_fmt, 1);
                    if (mDecodeContext->mMediaCallBack != NULL) {
                        mDecodeContext->mMediaCallBack->CallBackDecodeDataAudio(
                                out_buffer, size);
                    }

                }
            }
            av_packet_unref(mDecodeContext->mAVPacket);
        }
    }
    av_free(out_buffer);
    swr_free(&swrCtx);
    av_frame_free(&mDecodeContext->mAVFrame);
    avcodec_close(mDecodeContext->mAVCodecCtx);
    avformat_close_input(&mDecodeContext->mAVformat);
}


void *decodeStream(void *arg) {
    int i = decodeVideo();
}

int FFmpegEncodeStream::openStream(char *path) {
    pthread_t id;
    videoPath = (char *) malloc(strlen(path));
    stpcpy(videoPath, path);
    mmMediaCallBack = mMediaCallBack;
    avcodec_register_all();
    avformat_network_init();
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
    free(videoPath);
    free(mmMediaCallBack);
    for (int i = 0; i < sizeof(pthread_ts) / sizeof(pthread_ts[0]); i++) {
        if (pthread_ts[i] != -1) {
            int result = pthread_join(pthread_ts[i], NULL);
        }
    }
}
