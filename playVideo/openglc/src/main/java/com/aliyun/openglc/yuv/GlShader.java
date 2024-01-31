/*
 *  Copyright 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

package com.aliyun.openglc.yuv;

import android.opengl.GLES20;

import java.nio.FloatBuffer;

// Helper class for handling OpenGL shaders and shader programs.
public class GlShader {
  private static final String TAG = "GlShader";

  private static int compileShader(int shaderType, String source) {
    final int shader = GLES20.glCreateShader(shaderType);
    if (shader == 0) {
      throw new RuntimeException("glCreateShader() failed. GLES20 error: " + GLES20.glGetError());
    }
    //将源码添加到shader并编译之
    GLES20.glShaderSource(shader, source);
    GLES20.glCompileShader(shader);
    int[] compileStatus = new int[] {GLES20.GL_FALSE};
    GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compileStatus, 0);
    if (compileStatus[0] != GLES20.GL_TRUE) {
//      Logging.e(
//          TAG, "Compile error " + GLES20.glGetShaderInfoLog(shader) + " in shader:\n" + source);
      throw new RuntimeException(GLES20.glGetShaderInfoLog(shader));
    }
    GlUtil.checkNoGLES2Error("compileShader");
    return shader;
  }

  private int program;

  public GlShader(String vertexSource, String fragmentSource) {
    final int vertexShader = compileShader(GLES20.GL_VERTEX_SHADER, vertexSource);
    final int fragmentShader = compileShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
    program = GLES20.glCreateProgram();
    if (program == 0) {
      throw new RuntimeException("glCreateProgram() failed. GLES20 error: " + GLES20.glGetError());
    }
    // 将vertexShader shader添加到program
    GLES20.glAttachShader(program, vertexShader);
    // 将fragmentShader shader添加到program
    GLES20.glAttachShader(program, fragmentShader);
    // 创建可执行的 OpenGL ES program（耗CPU）
    GLES20.glLinkProgram(program);
    int[] linkStatus = new int[] {GLES20.GL_FALSE};
    GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
    if (linkStatus[0] != GLES20.GL_TRUE) {
    //  Logging.e(TAG, "Could not link program: " + GLES20.glGetProgramInfoLog(program));
      throw new RuntimeException(GLES20.glGetProgramInfoLog(program));
    }
    // According to the documentation of glLinkProgram():
    // "After the link operation, applications are free to modify attached shader objects, compile
    // attached shader objects, detach shader objects, delete shader objects, and attach additional
    // shader objects. None of these operations affects the information log or the program that is
    // part of the program object."
    // But in practice, detaching shaders from the program seems to break some devices. Deleting the
    // shaders are fine however - it will delete them when they are no longer attached to a program.
    GLES20.glDeleteShader(vertexShader);
    GLES20.glDeleteShader(fragmentShader);
    GlUtil.checkNoGLES2Error("Creating GlShader");
  }

  public int getAttribLocation(String label) {
    if (program == -1) {
      throw new RuntimeException("The program has been released");
    }
    int location = GLES20.glGetAttribLocation(program, label);
    if (location < 0) {
      throw new RuntimeException("Could not locate '" + label + "' in program");
    }
    return location;
  }

  /**
   * Enable and upload a vertex array for attribute |label|. The vertex data is specified in
   * |buffer| with |dimension| number of components per vertex.
   */
  public void setVertexAttribArray(String label, int dimension, FloatBuffer buffer) {
    setVertexAttribArray(label, dimension, 0 /* stride */, buffer);
  }

  /**
   * Enable and upload a vertex array for attribute |label|. The vertex data is specified in
   * |buffer| with |dimension| number of components per vertex and specified |stride|.
   */
  public void setVertexAttribArray(String label, int dimension, int stride, FloatBuffer buffer) {
    if (program == -1) {
      throw new RuntimeException("The program has been released");
    }
    int location = getAttribLocation(label);
    // 允许使用顶点坐标数组
    GLES20.glEnableVertexAttribArray(location);
    // 顶点坐标传递到顶点着色器
    GLES20.glVertexAttribPointer(location, dimension, GLES20.GL_FLOAT, false, stride, buffer);
    GlUtil.checkNoGLES2Error("setVertexAttribArray");
  }

  /**
  /**
   //获取shader里面uniform变量的地址
   * @param label
   * @return
   */
  public int getUniformLocation(String label) {
    if (program == -1) {
      throw new RuntimeException("The program has been released");
    }
    // 获取指向着色器中label的index
    int location = GLES20.glGetUniformLocation(program, label);
    if (location < 0) {
      throw new RuntimeException("Could not locate uniform '" + label + "' in program");
    }
    return location;
  }

  public void useProgram() {
    if (program == -1) {
      throw new RuntimeException("The program has been released");
    }
    // 使用shader程序
    GLES20.glUseProgram(program);
    GlUtil.checkNoGLES2Error("glUseProgram");
  }

  public void release() {
   // Logging.d(TAG, "Deleting shader.");
    // Delete program, automatically detaching any shaders from it.
    if (program != -1) {
      GLES20.glDeleteProgram(program);
      program = -1;
    }
  }
}
