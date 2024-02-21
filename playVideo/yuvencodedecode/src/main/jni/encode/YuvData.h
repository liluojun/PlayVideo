//
// Created by hjt on 2024/2/20.
//

#ifndef PLAYVIDEO_YUVDATA_H
#define PLAYVIDEO_YUVDATA_H

#include "stdint.h"

struct YuvData {
    uint8_t *y;
    uint8_t *u;
    uint8_t *v;
};
#endif //PLAYVIDEO_YUVDATA_H