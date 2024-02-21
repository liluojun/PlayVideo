//
// Created by hjt on 2024/1/26.
//

#include "VideoRender.h"

using namespace std;

#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",__VA_ARGS__)
#ifdef __cplusplus

VideoRender::VideoRender() {

}

long VideoRender::creatSurface(ANativeWindow *mWindow, jint w, jint h) {
    m = new RenderWindow();
    m->mWindow = mWindow;
    m->mEglEnvironment = new EglEnvironment();
    m->mEGLSurface = m->mEglEnvironment->createWindowSurface(m->mWindow);
    m->mGlDraw = new GlDraw();
    m->handel = -1;
    m->h = h;
    m->w = w;
    return m->handel;
}

int VideoRender::destorySurface() {

    if (NULL != m) {
        ANativeWindow_release(m->mWindow);
        m->mEglEnvironment->releaseSurface(m->mEGLSurface);
        m->mGlDraw->release();
        delete m;
        return 0;
    } else {
        return -1;
    }
}

int VideoRender::changeSurfaceSize(int w, int h) {

    if (NULL != m) {
        m->h = h;
        m->w = w;
        m->mEglEnvironment->changeSurfaceSize(w,h);
        return 0;
    } else {
        return -1;
    }
}

#endif
#ifdef __cplusplus


#endif