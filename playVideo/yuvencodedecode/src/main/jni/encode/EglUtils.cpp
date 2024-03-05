//
// Created by hjt on 2024/2/4.
//
#ifndef EGLUTILS_C
#define EGLUTILS_C
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",__VA_ARGS__)
#include <EGL/egl.h>
#include <string.h>
#include <android/log.h>
#include "GlRendering.h"
#include <GLES2/gl2.h>
#include <iostream>

static std::string getEGLErrorString(int error) {

    switch (error) {
        case EGL_SUCCESS:
            return "EGL_SUCCESS";
        case EGL_NOT_INITIALIZED:
            return "EGL_NOT_INITIALIZED";
        case EGL_BAD_ACCESS:
            return "EGL_BAD_ACCESS";
        case EGL_BAD_ALLOC:
            return "EGL_BAD_ALLOC";
        case EGL_BAD_ATTRIBUTE:
            return "EGL_BAD_ATTRIBUTE";
        case EGL_BAD_CONFIG:
            return "EGL_BAD_CONFIG";
        case EGL_BAD_CONTEXT:
            return "EGL_BAD_CONTEXT";
        case EGL_BAD_CURRENT_SURFACE:
            return "EGL_BAD_CURRENT_SURFACE";
        case EGL_BAD_DISPLAY:
            return "EGL_BAD_DISPLAY";
        case EGL_BAD_MATCH:
            return "EGL_BAD_MATCH";
        case EGL_BAD_NATIVE_PIXMAP:
            return "EGL_BAD_NATIVE_PIXMAP";
        case EGL_BAD_NATIVE_WINDOW:
            return "EGL_BAD_NATIVE_WINDOW";
        case EGL_BAD_PARAMETER:
            return "EGL_BAD_PARAMETER";
        case EGL_BAD_SURFACE:
            return "EGL_BAD_SURFACE";
        case EGL_CONTEXT_LOST:
            return "EGL_CONTEXT_LOST";
        default:
            return "default";
    }

}

static void checkNoGLES2Error(char *msg) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        LOGE("checkNoGLES2Error %s: GLES20 error: %d", msg, error);
    }

}
static GLuint generateTexture(int target) {
    GLuint textureArray[1] ;
    glGenTextures(1, textureArray);
    GLuint textureId = textureArray[0];
    glBindTexture(target, textureId);
    glTexParameterf(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    checkNoGLES2Error("generateTexture");
    return textureId;
}
#endif