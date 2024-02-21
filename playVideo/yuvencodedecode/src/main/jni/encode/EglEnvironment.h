//
// Created by hjt on 2024/2/3.
//

#ifndef PLAYVIDEO_EGLENVIRONMENT_H
#define PLAYVIDEO_EGLENVIRONMENT_H
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",__VA_ARGS__)
#define FLAG_RECORDABLE 0x01

#include <android/log.h>
#include <android/native_window.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#include <iostream>

class EglEnvironment {
private:
    EGLDisplay mEGLDisplay = EGL_NO_DISPLAY;
    EGLConfig mEGLConfig = NULL;
    EGLContext mEGLContext = EGL_NO_CONTEXT;
    int mGlVersion = 0;
    EGLSurface mEGLSurface = NULL;

    // 查找合适的EGLConfig
    EGLConfig getConfig(int flags, int version);

public:
    EglEnvironment();

    ~EglEnvironment();

    EglEnvironment(EGLContext sharedContext, int flags);

    bool init(EGLContext sharedContext, int flags);

    bool checkPrepare();

    void swapBuffers();

    // 释放资源
    void release();

    // 获取EglContext
    EGLContext getEGLContext();

    // 销毁Surface
    void releaseSurface(EGLSurface eglSurface);

    //  创建EGLSurface
    EGLSurface createWindowSurface(ANativeWindow *surface);

    // 切换到当前上下文
    void makeCurrent(EGLSurface eglSurface);

    // 执行查询
    int querySurface(EGLSurface eglSurface, int what);

    // 获取当前的GLES 版本号
    int getGlVersion();

    // 检查是否出错
    void checkEglError(const char *msg);

    void changeSurfaceSize(int i, int i1);
};


#endif //PLAYVIDEO_EGLENVIRONMENT_H
