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
    return 0;
}
#ifdef __cplusplus
}
#endif

