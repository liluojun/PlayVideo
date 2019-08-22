/*
 *  Copyright 2015 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

package sc.playvideo.com.yuvencodedecode.yuv;

import android.content.Context;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.opengles.GL10;

/**
 * Some OpenGL static utility functions.
 */
public class GlUtil {
  private GlUtil() {}

  // Assert that no OpenGL ES 2.0 error has been raised.
  public static void checkNoGLES2Error(String msg) {
    int error = GLES20.glGetError();
    if (error != GLES20.GL_NO_ERROR) {
//      throw new RuntimeException(msg + ": GLES20 error: " + error);
      Log.i("checkNoGLES2Error",msg + ": GLES20 error: " + error);
    }
  }

  public static FloatBuffer createFloatBuffer(float[] coords) {
    // Allocate a direct ByteBuffer, using 4 bytes per float, and copy coords into it.
    ByteBuffer bb = ByteBuffer.allocateDirect(coords.length * 4);
    bb.order(ByteOrder.nativeOrder());
    FloatBuffer fb = bb.asFloatBuffer();
    fb.put(coords);
    fb.position(0);
    return fb;
  }

  /**
   * Generate texture with standard parameters.
   */
  public static int generateTexture(int target) {
    final int textureArray[] = new int[1];
    GLES20.glGenTextures(1, textureArray, 0);
    final int textureId = textureArray[0];
    GLES20.glBindTexture(target, textureId);
    GLES20.glTexParameterf(target, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
    GLES20.glTexParameterf(target, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
    GLES20.glTexParameterf(target, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
    GLES20.glTexParameterf(target, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
    checkNoGLES2Error("generateTexture");
    return textureId;
  }

  public static int createOESTextureObject() {
    int[] tex = new int[1];
    GLES20.glGenTextures(1, tex, 0);
    GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, tex[0]);
    GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
            GL10.GL_TEXTURE_MIN_FILTER, GL10.GL_NEAREST);
    GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
            GL10.GL_TEXTURE_MAG_FILTER, GL10.GL_LINEAR);
    GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
            GL10.GL_TEXTURE_WRAP_S, GL10.GL_CLAMP_TO_EDGE);
    GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES,
            GL10.GL_TEXTURE_WRAP_T, GL10.GL_CLAMP_TO_EDGE);
    GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, 0);
    return tex[0];
  }

  public static String readShaderFromResource(Context context, int resourceId) {
    StringBuilder builder = new StringBuilder();
    InputStream is = null;
    InputStreamReader isr = null;
    BufferedReader br = null;
    try {
      is = context.getResources().openRawResource(resourceId);
      isr = new InputStreamReader(is);
      br = new BufferedReader(isr);
      String line;
      while ((line = br.readLine()) != null) {
        builder.append(line + "\n");
      }
    } catch (IOException e) {
      e.printStackTrace();
    } finally {
      try {
        if (is != null) {
          is.close();
          is = null;
        }
        if (isr != null) {
          isr.close();
          isr = null;
        }
        if (br != null) {
          br.close();
          br = null;
        }
      } catch (IOException e) {
        e.printStackTrace();
      }
    }
    return builder.toString();
  }
}
