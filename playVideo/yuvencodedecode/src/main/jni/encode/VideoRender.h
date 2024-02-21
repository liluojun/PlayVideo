//
// Created by hjt on 2024/1/26.
//

#include <jni.h>
/* Header for class media_jni_MediaNative */
#ifndef _Included_VideoRender
#define _Included_VideoRender
#ifdef __cplusplus

#include <iostream>
#include <map>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <android/log.h>
#include "EglEnvironment.h"
#include "GlRendering.h"
#include "GlDraw.h"

using namespace std;

#endif
typedef struct {
    ANativeWindow *mWindow = NULL;
    long handel = 0;
    int w = 0;
    int h = 0;
    EGLSurface mEGLSurface = NULL;
    GlDraw *mGlDraw = NULL;
    EglEnvironment *mEglEnvironment = NULL;

} RenderWindow;

class VideoRender {
public:
    VideoRender();

    long creatSurface(ANativeWindow *mWindow, int w, int h);

    int destorySurface();

    int changeSurfaceSize(int w, int h);

    RenderWindow *m;

};

#ifdef __cplusplus

#endif
#endif

