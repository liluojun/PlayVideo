//
// Created by hjt on 2024/2/19.
//

#include "GlThread.h"

GlThread::GlThread() {
    mRender = new VideoRender();
}


void GlThread::handleMessage(LooperMessage *msg) {
    if (!msg) {
        return;
    }
    switch (msg->what) {
        case kMsgSurfaceCreated:
            if (mRender) {
                mRender->creatSurface((ANativeWindow *) msg->obj, msg->arg1, msg->arg2);
            }
            break;

        case kMsgSurfaceChanged:
            if (mRender) {
                mRender->changeSurfaceSize(msg->arg1, msg->arg2);
            }
            break;

        case kMsgSurfaceDestroyed:
            if (mRender) {
                mRender->destorySurface();
            }
            break;
        case kMsgYuvData:
            if (mRender) {
                if (mRender->m != NULL) {
                    glClearColor(1.0, 1.0, 1.0, 1.0);
                    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    GLuint yuvTextures[3];

                    mRender->m->mGlDraw->perparDrawYuv(msg->arg1, msg->arg2,
                                                       (YuvData *) msg->obj, yuvTextures);
                    delete ((YuvData *) msg->obj);
                    mRender->m->mGlDraw->drawYuv(yuvTextures, 0, 0, mRender->m->w, mRender->m->h);

                    mRender->m->mEglEnvironment->swapBuffers();
                }
            }
            break;
    }
}

GlThread::~GlThread() {
    delete mRender;
}