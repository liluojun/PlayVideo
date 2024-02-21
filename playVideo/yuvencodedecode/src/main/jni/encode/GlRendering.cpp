//
// Created by hjt on 2024/2/4.
//

#include "GlRendering.h"

GlRendering::GlRendering() {

}

GlRendering::GlRendering(char *vertexSource, char *fragmentSource) {
    GlShader(vertexSource, fragmentSource);
}

int compileShader(int shaderType, char *source) {
    GLuint shader = glCreateShader(shaderType);
    GLint compileStatus;
    if (shader == 0) {
        LOGE("glCreateShader() failed. GLES20 error:%d ", glGetError());
        return -1;
    }
    //将源码添加到shader并编译之

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        return -2;
    }
    checkNoGLES2Error("compileShader");
    return shader;
}


int GlRendering::GlShader(char *vertexSource, char *fragmentSource) {
    int vertexShader = compileShader(
            GL_VERTEX_SHADER, vertexSource);
    int fragmentShader = compileShader(
            GL_FRAGMENT_SHADER, fragmentSource);
    program = glCreateProgram();
    if (program == 0) {
        LOGE("glCreateShader() failed. GLES20 error:%d ", glGetError());
        return -1;
    }
// 将vertexShader shader添加到program
    glAttachShader(program, vertexShader);
// 将fragmentShader shader添加到program
    glAttachShader(program, fragmentShader);
// 创建可执行的 OpenGL ES program（耗CPU）
    glLinkProgram(program);
    GLint compileStatus=GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE) {
        //LOGE("glCreateShader() failed. GLES20 error:%d ",glGetProgramInfoLog(program));
        return -2;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    checkNoGLES2Error("Creating GlShader");
    return 0;
}

GLint GlRendering::getAttribLocation(char *label) {
    if (program == -1) {
        LOGE("The program has been released");
        return -1;
    }
    GLint location = glGetAttribLocation(program, label);
    if (location < 0) {
        LOGE("Could not locate %d in program", label);
        return -2;
    }
    return location;
}


/**
 * Enable and upload a vertex array for attribute |label|. The vertex data is specified in
 * |buffer| with |dimension| number of components per vertex and specified |stride|.
 */
void GlRendering::setVertexAttribArray(GLchar *label,
                                       int dimension,
                                       int stride, const void *pointer) {
    if (program == -1) {
        LOGE("The program has been released");
        return;
    }
    GLint location = getAttribLocation(label);
// 允许使用顶点坐标数组
    glEnableVertexAttribArray(location);
// 顶点坐标传递到顶点着色器
    glVertexAttribPointer(location, dimension, GL_FLOAT, false, stride, pointer);
}

/**
/**
 //获取shader里面uniform变量的地址
 * @param label
 * @return
 */
GLint GlRendering::getUniformLocation(GLchar *label) {
    if (program == -1) {
        LOGE("The program has been released");
        return -1;
    }
// 获取指向着色器中label的index
    GLint location = glGetUniformLocation(program, label);
    if (location < 0) {
        LOGE("Could not locate uniform %d in program", location);
        return -1;
    }
    return location;
}

void GlRendering::useProgram() {
    if (program == -1) {
        LOGE("The program has been released");
        return;
    }
    // 使用shader程序
    glUseProgram(program);
    checkNoGLES2Error("glUseProgram");
}

void GlRendering::release() {
    // Delete program, automatically detaching any shaders from it.
    if (program != -1) {
        glDeleteProgram(program);
        program = -1;
    }
}