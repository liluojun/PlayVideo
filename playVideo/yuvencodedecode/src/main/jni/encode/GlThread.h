//
// Created by hjt on 2024/2/19.
//

#ifndef PLAYVIDEO_GLTHREAD_H
#define PLAYVIDEO_GLTHREAD_H


#include "Looper.h"
#include "VideoRender.h"
#include "YuvData.h"

enum {
    kMsgSurfaceCreated,
    kMsgSurfaceChanged,
    kMsgSurfaceDestroyed,
    kMsgYuvData
};

class GlThread : public Looper {

public:
    GlThread();

    virtual ~GlThread();

    virtual void handleMessage(LooperMessage *msg);

private:
    VideoRender *mRender;

};


#endif