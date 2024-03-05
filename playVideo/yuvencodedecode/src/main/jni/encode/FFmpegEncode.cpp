//
// Created by Administrator on 2019/10/20.
//
#include "FFmpegEncode.h"
#include "VideoRender.h"

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",__VA_ARGS__)
#ifdef __cplusplus
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"

#endif
int FFmpegEncode::creatFFmpeg(AVCodecID id) {
    avcodec_register_all();
    mAVCodec = avcodec_find_decoder(id);
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
int FFmpegEncode::initAVCodecContext(int width, int heigth, int fps, AVPixelFormat pixFmt) {
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
    return 0;
}
int FFmpegEncode::encodeFFmpeg(uint8_t *framedata, int framelen,//input
                               uint8_t *outputY, uint8_t *outputU, uint8_t *outputV, int *width,
                               int *height,
                               AVPixelFormat pixfmt) {
    if (mAVPacket == NULL) {
        printf("encodeFFmpeg NULL\n");
        return -1;
    }
    mAVPacket->data = framedata;
    mAVPacket->size = framelen;
    while (mAVPacket->size > 0) {
        if (avcodec_send_packet(mAVCodecContext, mAVPacket)) {
            printf("%s %d avcodec_send_packet fail\n", __func__, __LINE__);
            return -2;
        }
        if (avcodec_receive_frame(mAVCodecContext, mAVFrame)) {
            printf("%s %d avcodec_receive_frame fail\n", __func__, __LINE__);
            return -3;
        }

        switch (mAVCodecContext->pix_fmt) {
            case AV_PIX_FMT_YUV420P: {
                int size = mAVCodecContext->width * mAVCodecContext->height;
                YuvData *yuvData = new YuvData();
                yuvData->y = new uint8_t[size];
                memset(yuvData->y, 0, sizeof(uint8_t) * size);
                yuvData->u = new uint8_t[size / 4];
                memset(yuvData->u, 0, sizeof(uint8_t) * size / 4);
                yuvData->v = new uint8_t[size / 4];
                memset(yuvData->v, 0, sizeof(uint8_t) * size / 4);
                memcpy(yuvData->y, mAVFrame->data[0],
                       size);
                memcpy(yuvData->u, mAVFrame->data[1],
                       size / 4);
                memcpy(yuvData->v, mAVFrame->data[2],
                       size / 4);

                renderToC(mAVCodecContext->width, mAVCodecContext->height, yuvData);
                break;
            }
            case AV_PIX_FMT_NV21: {
                YuvData *yuvData = new YuvData();
                memcpy(yuvData->y, mAVFrame->data[0],
                       mAVCodecContext->width * mAVCodecContext->height);
                memcpy(yuvData->u, mAVFrame->data[1],
                       mAVCodecContext->width * mAVCodecContext->height / 4);
                memcpy(yuvData->v, mAVFrame->data[2],
                       mAVCodecContext->width * mAVCodecContext->height / 4);

                renderToC(mAVCodecContext->width, mAVCodecContext->height, yuvData);
                break;
            }
            default: {
                printf("mAVCodecContext->pix_fmt Failure to identify\n");
                return -4;
            }
            case AV_PIX_FMT_NONE:
                break;
            case AV_PIX_FMT_YUYV422:
                break;
            case AV_PIX_FMT_RGB24:
                break;
            case AV_PIX_FMT_BGR24:
                break;
            case AV_PIX_FMT_YUV422P:
                break;
            case AV_PIX_FMT_YUV444P:
                break;
            case AV_PIX_FMT_YUV410P:
                break;
            case AV_PIX_FMT_YUV411P:
                break;
            case AV_PIX_FMT_GRAY8:
                break;
            case AV_PIX_FMT_MONOWHITE:
                break;
            case AV_PIX_FMT_MONOBLACK:
                break;
            case AV_PIX_FMT_PAL8:
                break;
            case AV_PIX_FMT_YUVJ420P:
                break;
            case AV_PIX_FMT_YUVJ422P:
                break;
            case AV_PIX_FMT_YUVJ444P:
                break;
            case AV_PIX_FMT_UYVY422:
                break;
            case AV_PIX_FMT_UYYVYY411:
                break;
            case AV_PIX_FMT_BGR8:
                break;
            case AV_PIX_FMT_BGR4:
                break;
            case AV_PIX_FMT_BGR4_BYTE:
                break;
            case AV_PIX_FMT_RGB8:
                break;
            case AV_PIX_FMT_RGB4:
                break;
            case AV_PIX_FMT_RGB4_BYTE:
                break;
            case AV_PIX_FMT_NV12:
                break;
            case AV_PIX_FMT_ARGB:
                break;
            case AV_PIX_FMT_RGBA:
                break;
            case AV_PIX_FMT_ABGR:
                break;
            case AV_PIX_FMT_BGRA:
                break;
            case AV_PIX_FMT_GRAY16BE:
                break;
            case AV_PIX_FMT_GRAY16LE:
                break;
            case AV_PIX_FMT_YUV440P:
                break;
            case AV_PIX_FMT_YUVJ440P:
                break;
            case AV_PIX_FMT_YUVA420P:
                break;
            case AV_PIX_FMT_RGB48BE:
                break;
            case AV_PIX_FMT_RGB48LE:
                break;
            case AV_PIX_FMT_RGB565BE:
                break;
            case AV_PIX_FMT_RGB565LE:
                break;
            case AV_PIX_FMT_RGB555BE:
                break;
            case AV_PIX_FMT_RGB555LE:
                break;
            case AV_PIX_FMT_BGR565BE:
                break;
            case AV_PIX_FMT_BGR565LE:
                break;
            case AV_PIX_FMT_BGR555BE:
                break;
            case AV_PIX_FMT_BGR555LE:
                break;
            case AV_PIX_FMT_VAAPI_MOCO:
                break;
            case AV_PIX_FMT_VAAPI_IDCT:
                break;
            case AV_PIX_FMT_VAAPI_VLD:
                break;
            case AV_PIX_FMT_YUV420P16LE:
                break;
            case AV_PIX_FMT_YUV420P16BE:
                break;
            case AV_PIX_FMT_YUV422P16LE:
                break;
            case AV_PIX_FMT_YUV422P16BE:
                break;
            case AV_PIX_FMT_YUV444P16LE:
                break;
            case AV_PIX_FMT_YUV444P16BE:
                break;
            case AV_PIX_FMT_DXVA2_VLD:
                break;
            case AV_PIX_FMT_RGB444LE:
                break;
            case AV_PIX_FMT_RGB444BE:
                break;
            case AV_PIX_FMT_BGR444LE:
                break;
            case AV_PIX_FMT_BGR444BE:
                break;
            case AV_PIX_FMT_YA8:
                break;
            case AV_PIX_FMT_BGR48BE:
                break;
            case AV_PIX_FMT_BGR48LE:
                break;
            case AV_PIX_FMT_YUV420P9BE:
                break;
            case AV_PIX_FMT_YUV420P9LE:
                break;
            case AV_PIX_FMT_YUV420P10BE:
                break;
            case AV_PIX_FMT_YUV420P10LE:
                break;
            case AV_PIX_FMT_YUV422P10BE:
                break;
            case AV_PIX_FMT_YUV422P10LE:
                break;
            case AV_PIX_FMT_YUV444P9BE:
                break;
            case AV_PIX_FMT_YUV444P9LE:
                break;
            case AV_PIX_FMT_YUV444P10BE:
                break;
            case AV_PIX_FMT_YUV444P10LE:
                break;
            case AV_PIX_FMT_YUV422P9BE:
                break;
            case AV_PIX_FMT_YUV422P9LE:
                break;
            case AV_PIX_FMT_GBRP:
                break;
            case AV_PIX_FMT_GBRP9BE:
                break;
            case AV_PIX_FMT_GBRP9LE:
                break;
            case AV_PIX_FMT_GBRP10BE:
                break;
            case AV_PIX_FMT_GBRP10LE:
                break;
            case AV_PIX_FMT_GBRP16BE:
                break;
            case AV_PIX_FMT_GBRP16LE:
                break;
            case AV_PIX_FMT_YUVA422P:
                break;
            case AV_PIX_FMT_YUVA444P:
                break;
            case AV_PIX_FMT_YUVA420P9BE:
                break;
            case AV_PIX_FMT_YUVA420P9LE:
                break;
            case AV_PIX_FMT_YUVA422P9BE:
                break;
            case AV_PIX_FMT_YUVA422P9LE:
                break;
            case AV_PIX_FMT_YUVA444P9BE:
                break;
            case AV_PIX_FMT_YUVA444P9LE:
                break;
            case AV_PIX_FMT_YUVA420P10BE:
                break;
            case AV_PIX_FMT_YUVA420P10LE:
                break;
            case AV_PIX_FMT_YUVA422P10BE:
                break;
            case AV_PIX_FMT_YUVA422P10LE:
                break;
            case AV_PIX_FMT_YUVA444P10BE:
                break;
            case AV_PIX_FMT_YUVA444P10LE:
                break;
            case AV_PIX_FMT_YUVA420P16BE:
                break;
            case AV_PIX_FMT_YUVA420P16LE:
                break;
            case AV_PIX_FMT_YUVA422P16BE:
                break;
            case AV_PIX_FMT_YUVA422P16LE:
                break;
            case AV_PIX_FMT_YUVA444P16BE:
                break;
            case AV_PIX_FMT_YUVA444P16LE:
                break;
            case AV_PIX_FMT_VDPAU:
                break;
            case AV_PIX_FMT_XYZ12LE:
                break;
            case AV_PIX_FMT_XYZ12BE:
                break;
            case AV_PIX_FMT_NV16:
                break;
            case AV_PIX_FMT_NV20LE:
                break;
            case AV_PIX_FMT_NV20BE:
                break;
            case AV_PIX_FMT_RGBA64BE:
                break;
            case AV_PIX_FMT_RGBA64LE:
                break;
            case AV_PIX_FMT_BGRA64BE:
                break;
            case AV_PIX_FMT_BGRA64LE:
                break;
            case AV_PIX_FMT_YVYU422:
                break;
            case AV_PIX_FMT_YA16BE:
                break;
            case AV_PIX_FMT_YA16LE:
                break;
            case AV_PIX_FMT_GBRAP:
                break;
            case AV_PIX_FMT_GBRAP16BE:
                break;
            case AV_PIX_FMT_GBRAP16LE:
                break;
            case AV_PIX_FMT_QSV:
                break;
            case AV_PIX_FMT_MMAL:
                break;
            case AV_PIX_FMT_D3D11VA_VLD:
                break;
            case AV_PIX_FMT_CUDA:
                break;
            case AV_PIX_FMT_0RGB:
                break;
            case AV_PIX_FMT_RGB0:
                break;
            case AV_PIX_FMT_0BGR:
                break;
            case AV_PIX_FMT_BGR0:
                break;
            case AV_PIX_FMT_YUV420P12BE:
                break;
            case AV_PIX_FMT_YUV420P12LE:
                break;
            case AV_PIX_FMT_YUV420P14BE:
                break;
            case AV_PIX_FMT_YUV420P14LE:
                break;
            case AV_PIX_FMT_YUV422P12BE:
                break;
            case AV_PIX_FMT_YUV422P12LE:
                break;
            case AV_PIX_FMT_YUV422P14BE:
                break;
            case AV_PIX_FMT_YUV422P14LE:
                break;
            case AV_PIX_FMT_YUV444P12BE:
                break;
            case AV_PIX_FMT_YUV444P12LE:
                break;
            case AV_PIX_FMT_YUV444P14BE:
                break;
            case AV_PIX_FMT_YUV444P14LE:
                break;
            case AV_PIX_FMT_GBRP12BE:
                break;
            case AV_PIX_FMT_GBRP12LE:
                break;
            case AV_PIX_FMT_GBRP14BE:
                break;
            case AV_PIX_FMT_GBRP14LE:
                break;
            case AV_PIX_FMT_YUVJ411P:
                break;
            case AV_PIX_FMT_BAYER_BGGR8:
                break;
            case AV_PIX_FMT_BAYER_RGGB8:
                break;
            case AV_PIX_FMT_BAYER_GBRG8:
                break;
            case AV_PIX_FMT_BAYER_GRBG8:
                break;
            case AV_PIX_FMT_BAYER_BGGR16LE:
                break;
            case AV_PIX_FMT_BAYER_BGGR16BE:
                break;
            case AV_PIX_FMT_BAYER_RGGB16LE:
                break;
            case AV_PIX_FMT_BAYER_RGGB16BE:
                break;
            case AV_PIX_FMT_BAYER_GBRG16LE:
                break;
            case AV_PIX_FMT_BAYER_GBRG16BE:
                break;
            case AV_PIX_FMT_BAYER_GRBG16LE:
                break;
            case AV_PIX_FMT_BAYER_GRBG16BE:
                break;
            case AV_PIX_FMT_XVMC:
                break;
            case AV_PIX_FMT_YUV440P10LE:
                break;
            case AV_PIX_FMT_YUV440P10BE:
                break;
            case AV_PIX_FMT_YUV440P12LE:
                break;
            case AV_PIX_FMT_YUV440P12BE:
                break;
            case AV_PIX_FMT_AYUV64LE:
                break;
            case AV_PIX_FMT_AYUV64BE:
                break;
            case AV_PIX_FMT_VIDEOTOOLBOX:
                break;
            case AV_PIX_FMT_P010LE:
                break;
            case AV_PIX_FMT_P010BE:
                break;
            case AV_PIX_FMT_GBRAP12BE:
                break;
            case AV_PIX_FMT_GBRAP12LE:
                break;
            case AV_PIX_FMT_GBRAP10BE:
                break;
            case AV_PIX_FMT_GBRAP10LE:
                break;
            case AV_PIX_FMT_MEDIACODEC:
                break;
            case AV_PIX_FMT_GRAY12BE:
                break;
            case AV_PIX_FMT_GRAY12LE:
                break;
            case AV_PIX_FMT_GRAY10BE:
                break;
            case AV_PIX_FMT_GRAY10LE:
                break;
            case AV_PIX_FMT_P016LE:
                break;
            case AV_PIX_FMT_P016BE:
                break;
            case AV_PIX_FMT_D3D11:
                break;
            case AV_PIX_FMT_GRAY9BE:
                break;
            case AV_PIX_FMT_GRAY9LE:
                break;
            case AV_PIX_FMT_GBRPF32BE:
                break;
            case AV_PIX_FMT_GBRPF32LE:
                break;
            case AV_PIX_FMT_GBRAPF32BE:
                break;
            case AV_PIX_FMT_GBRAPF32LE:
                break;
            case AV_PIX_FMT_DRM_PRIME:
                break;
            case AV_PIX_FMT_OPENCL:
                break;
            case AV_PIX_FMT_GRAY14BE:
                break;
            case AV_PIX_FMT_GRAY14LE:
                break;
            case AV_PIX_FMT_GRAYF32BE:
                break;
            case AV_PIX_FMT_GRAYF32LE:
                break;
            case AV_PIX_FMT_YUVA422P12BE:
                break;
            case AV_PIX_FMT_YUVA422P12LE:
                break;
            case AV_PIX_FMT_YUVA444P12BE:
                break;
            case AV_PIX_FMT_YUVA444P12LE:
                break;
            case AV_PIX_FMT_NV24:
                break;
            case AV_PIX_FMT_NV42:
                break;
            case AV_PIX_FMT_NB:
                break;
        }

        return 0;
    }
}
int FFmpegEncode::renderToC(int width, int heigth, YuvData *yuvData) {

    if (mGlThread != NULL) {
        mGlThread->postMessage(kMsgYuvData, width, heigth, yuvData);
    }
    return -1;
}
void FFmpegEncode::unEncode() {
    avcodec_close(mAVCodecContext);
    av_free(mAVCodecContext);
    av_frame_free(&mAVFrame);
}
#ifdef __cplusplus
}


#endif

