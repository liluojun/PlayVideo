//
// Created by hjt on 2024/2/3.
//

#include "EglEnvironment.h"
#include "EglUtils.cpp"

EglEnvironment::EglEnvironment() {
    init(NULL, 0);
}

EglEnvironment::~EglEnvironment() {
    release();
}

bool EglEnvironment::checkPrepare() {
    if (mEGLDisplay == EGL_NO_DISPLAY || mEGLContext == EGL_NO_CONTEXT || mEGLConfig == NULL) {
        return false;
    } else {
        return true;
    }
}

bool EglEnvironment::init(EGLContext sharedContext, int flags) {
    if (mEGLDisplay != EGL_NO_DISPLAY) {
        LOGE("mEGLDisplay already use");
        return false;
    }
    if (sharedContext == NULL) {
        sharedContext = EGL_NO_CONTEXT;
    }
    mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mEGLDisplay == EGL_NO_DISPLAY) {
        LOGE("unable to get EGL14 display.\n");
        return false;
    }

    if (!eglInitialize(mEGLDisplay, 0, 0)) {
        mEGLDisplay = EGL_NO_DISPLAY;
        LOGE("unable to initialize EGL14");
        return false;
    }

    int configAttribs[] = {EGL_BUFFER_SIZE, 32,//
                           EGL_ALPHA_SIZE, 8, // 指定Alpha大小，以下四项实际上指定了像素格式
                           EGL_BLUE_SIZE, 8, // 指定B大小
                           EGL_GREEN_SIZE, 8,// 指定G大小
                           EGL_RED_SIZE, 8,// 指定RGB中的R大小（bits）
                           EGL_RENDERABLE_TYPE,
                           EGL_OPENGL_ES2_BIT,// 指定渲染api类别,这里或者是硬编码的4，或者是EGL14.EGL_OPENGL_ES2_BIT
                           EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE// 总是以EGL10.EGL_NONE结尾
    };
    int *numConfigs = new int[1];
    EGLConfig *configs = new EGLConfig[1];
    // 用于获取满足attributes的所有config，参数1、2其意明显，参数3用于存放输出的configs，参数4指定最多输出多少个config，参数5由EGL系统写入，表明满足attributes的config一共有多少个
    if (!eglChooseConfig(mEGLDisplay, configAttribs, configs, 1,
                         numConfigs)) {// 获取所有满足attributes的configs,并选择一个
        LOGE("eglChooseConfig failed :%s ", getEGLErrorString(eglGetError()).c_str());
        return false;
    }
    if (mEGLContext == EGL_NO_CONTEXT) {
        EGLConfig config = getConfig(flags, 2);
        int attrib2_list[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };
        EGLContext context = eglCreateContext(mEGLDisplay, config,
                                              sharedContext, attrib2_list);
        checkEglError("eglCreateContext");
        if (eglGetError() == EGL_SUCCESS) {
            mEGLConfig = config;
            mEGLContext = context;
            mGlVersion = 2;
            return true;
        } else {
            return false;
        }
    }

}

// 释放资源
void EglEnvironment::release() {
    if (mEGLDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(mEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(mEGLDisplay, mEGLContext);
        eglReleaseThread();
        eglTerminate(mEGLDisplay);
    }

    mEGLDisplay = EGL_NO_DISPLAY;
    mEGLContext = EGL_NO_CONTEXT;
    mEGLConfig = NULL;
}

// 获取EglContext
EGLContext EglEnvironment::getEGLContext() {
    return mEGLContext;
}

// 销毁Surface
void EglEnvironment::releaseSurface(EGLSurface eglSurface) {
    if (mEGLDisplay != NULL && eglSurface != NULL) {
        eglDestroySurface(mEGLDisplay, eglSurface);
        mEGLSurface = NULL;
    }
}

//  创建EGLSurface
EGLSurface EglEnvironment::createWindowSurface(ANativeWindow *surface) {
    EGLSurface mEGLSurface = eglCreateWindowSurface(mEGLDisplay, mEGLConfig,
                                                    surface,// 负责对Android Surface的管理
                                                    NULL// Surface属性
    );
    this->mEGLSurface = mEGLSurface;
    makeCurrent(mEGLSurface);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return mEGLSurface;
}

// 切换到当前上下文
void EglEnvironment::makeCurrent(EGLSurface eglSurface) {
    EGLBoolean result = eglMakeCurrent(mEGLDisplay, eglSurface, eglSurface, mEGLContext);
    LOGE("makeCurrent result= %b", result);

}

// 执行查询
int EglEnvironment::querySurface(EGLSurface eglSurface, int what) {
    int value;
    eglQuerySurface(mEGLContext, eglSurface, what, &value);
    return value;
}

// 检查是否出错
void EglEnvironment::checkEglError(const char *msg) {
    int error;
    if ((error = eglGetError()) != EGL_SUCCESS) {
        LOGE("%s: EGL error: %x", msg, error);
    }
}

EGLConfig EglEnvironment::getConfig(int flags, int version) {
    int renderableType = EGL_OPENGL_ES2_BIT;
    int attribList[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            //EGL_DEPTH_SIZE, 16,
            //EGL_STENCIL_SIZE, 8,
            EGL_RENDERABLE_TYPE, renderableType,
            EGL_NONE, 0,      // placeholder for recordable [@-3]
            EGL_NONE
    };
    int length = sizeof(attribList) / sizeof(attribList[0]);
    if ((flags & FLAG_RECORDABLE) != 0) {
        attribList[length - 3] = EGL_RECORDABLE_ANDROID;
        attribList[length - 2] = 1;
    }
    EGLConfig configs = NULL;
    int numConfigs;
    if (!eglChooseConfig(mEGLDisplay, attribList, &configs, 1, &numConfigs)) {
        LOGE("unable to find RGB8888 / %d  EGLConfig", version);
        return NULL;
    }
    return configs;
}

void EglEnvironment::swapBuffers() {
    eglSwapBuffers(mEGLDisplay, mEGLSurface);
}

void EglEnvironment::changeSurfaceSize(int width, int height) {
    glViewport(0, 0, width, height);
}
