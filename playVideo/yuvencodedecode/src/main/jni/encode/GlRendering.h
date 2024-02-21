//
// Created by hjt on 2024/2/4.
//

#ifndef PLAYVIDEO_GLRENDERING_H
#define PLAYVIDEO_GLRENDERING_H
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"ffmpeg",__VA_ARGS__)

#include "EglUtils.cpp"
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>

class GlRendering {

public:


    int GlShader(char *vertexSource, char *fragmentSource);

    void setVertexAttribArray(GLchar *label,
                              int dimension,
                              int stride, const void *pointer);

    GLint getAttribLocation(char *label);

    GLint getUniformLocation(GLchar *label);
    GlRendering();
    GlRendering(char *vertexSource, char *fragmentSource);
    void useProgram();

    void release();
private:
    GLint program = -1;
};


#endif //PLAYVIDEO_GLRENDERING_H
