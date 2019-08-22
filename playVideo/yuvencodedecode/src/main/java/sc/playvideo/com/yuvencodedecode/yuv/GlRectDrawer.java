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

import android.graphics.Matrix;
import android.graphics.Point;
import android.opengl.GLES20;
import android.support.annotation.Nullable;

import java.nio.FloatBuffer;
import java.util.IdentityHashMap;
import java.util.Map;

/**
 * Helper class to draw an opaque quad on the target viewport location. Rotation, mirror, and
 * cropping is specified using a 4x4 texture coordinate transform matrix. The frame input can either
 * be an OES texture or YUV textures in I420 format. The GL state must be preserved between draw
 * calls, this is intentional to maximize performance. The function release() must be called
 * manually to free the resources held by this object.
 */
public class GlRectDrawer implements RendererCommon.GlDrawer {
    // clang-format off
    // Simple vertex shader, used for both YUV and OES.
    private static final String VERTEX_SHADER_STRING =
            "varying vec2 interp_tc;\n"
                    + "attribute vec4 in_pos;\n"
                    + "attribute vec4 in_tc;\n"
                    + "\n"
                    + "uniform mat4 texMatrix;\n"
                    + "\n"
                    + "void main() {\n"
                    + "    gl_Position = in_pos;\n"
                    + "    interp_tc = (texMatrix * in_tc).xy;\n"
                    + "}\n";

    private static final String YUV_FRAGMENT_SHADER_STRING =
            "precision mediump float;\n"
                    + "varying vec2 interp_tc;\n"
                    + "\n"
                    + "uniform sampler2D y_tex;\n"
                    + "uniform sampler2D u_tex;\n"
                    + "uniform sampler2D v_tex;\n"
                    + "\n"
                    + "void main() {\n"
                    // CSC according to http://www.fourcc.org/fccyvrgb.php
                    + "  float y = texture2D(y_tex, interp_tc).r;\n"
                    + "  float u = texture2D(u_tex, interp_tc).r - 0.5;\n"
                    + "  float v = texture2D(v_tex, interp_tc).r - 0.5;\n"
                    + "  gl_FragColor = vec4(y + 1.403 * v, "
                    + "                      y - 0.344 * u - 0.714 * v, "
                    + "                      y + 1.77 * u, 1);\n"
                    + "}\n";

    private static final String RGB_FRAGMENT_SHADER_STRING =
            "precision mediump float;\n"
                    + "varying vec2 interp_tc;\n"
                    + "\n"
                    + "uniform sampler2D rgb_tex;\n"
                    + "\n"
                    + "void main() {\n"
                    + "  gl_FragColor = texture2D(rgb_tex, interp_tc);\n"
                    + "}\n";

    private static final String OES_FRAGMENT_SHADER_STRING =
            "#extension GL_OES_EGL_image_external : require\n"
                    + "precision mediump float;\n"
                    + "varying vec2 interp_tc;\n"
                    + "\n"
                    + "uniform samplerExternalOES oes_tex;\n"
                    + "\n"
                    + "void main() {\n"
                    + "  gl_FragColor = texture2D(oes_tex, interp_tc);\n"
                    + "}\n";
    // clang-format on

    // Vertex coordinates in Normalized Device Coordinates, i.e. (-1, -1) is bottom-left and (1, 1) is
    // top-right.
    private static final FloatBuffer FULL_RECTANGLE_BUF = GlUtil.createFloatBuffer(new float[]{
            -1.0f, -1.0f, // Bottom left.
            1.0f, -1.0f, // Bottom right.
            -1.0f, 1.0f, // Top left.
            1.0f, 1.0f, // Top right.
    });

    // Texture coordinates - (0, 0) is bottom-left and (1, 1) is top-right.
    private static final FloatBuffer FULL_RECTANGLE_TEX_BUF = GlUtil.createFloatBuffer(new float[]{
            0.0f, 0.0f, // Bottom left.
            1.0f, 0.0f, // Bottom right.
            0.0f, 1.0f, // Top left.
            1.0f, 1.0f // Top right.
//            0.0f, 1.0f,//左上角
//            1.0f, 1.0f,//右上角
//            0.0f, 0.0f,//左下角
//            1.0f, 0.0f,//右下角
    });

    private static class Shader {
        public final GlShader glShader;
        public final int texMatrixLocation;

        public Shader(String fragmentShader) {
            this.glShader = new GlShader(VERTEX_SHADER_STRING, fragmentShader);
            this.texMatrixLocation = glShader.getUniformLocation("texMatrix");
        }
    }

    // The keys are one of the fragments shaders above.
    private final Map<String, Shader> shaders = new IdentityHashMap<String, Shader>();

    /**
     * Draw an OES texture frame with specified texture transformation matrix. Required resources are
     * allocated at the first call to this function.
     */
    @Override
    public void drawOes(int oesTextureId, float[] texMatrix, int frameWidth, int frameHeight,
                        int viewportX, int viewportY, int viewportWidth, int viewportHeight) {
//        prepareShader(OES_FRAGMENT_SHADER_STRING, texMatrix);
//        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
//        // updateTexImage() may be called from another thread in another EGL context, so we need to
//        // bind/unbind the texture in each draw call so that GLES understads it's a new texture.
//        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, oesTextureId);
//        drawRectangle(viewportX, viewportY, viewportWidth, viewportHeight);
//        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, 0);
    }

    /**
     * Draw a RGB(A) texture frame with specified texture transformation matrix. Required resources
     * are allocated at the first call to this function.
     */
    @Override
    public void drawRgb(int textureId, float[] texMatrix, int frameWidth, int frameHeight,
                        int viewportX, int viewportY, int viewportWidth, int viewportHeight) {
//        prepareShader(RGB_FRAGMENT_SHADER_STRING, texMatrix);
//        GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
//        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);
//        drawRectangle(viewportX, viewportY, viewportWidth, viewportHeight);
//        // Unbind the texture as a precaution.
//        GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
    }

    @Override
    public void drawYuv(int[] yuvTextures, float[] texMatrix, int frameWidth, int frameHeight, int viewportX, int viewportY, int viewportWidth, int viewportHeight) {

    }

    private void calculateTransformedRenderSize(
            int frameWidth, int frameHeight, @Nullable Matrix renderMatrix) {
        if (renderMatrix == null) {
            renderWidth = frameWidth;
            renderHeight = frameHeight;
            return;
        }
        // Transform the texture coordinates (in the range [0, 1]) according to |renderMatrix|.
        renderMatrix.mapPoints(dstPoints, srcPoints);

        // Multiply with the width and height to get the positions in terms of pixels.
        for (int i = 0; i < 3; ++i) {
            dstPoints[i * 2 + 0] *= frameWidth;
            dstPoints[i * 2 + 1] *= frameHeight;
        }

        // Get the length of the sides of the transformed rectangle in terms of pixels.
        renderWidth = distance(dstPoints[0], dstPoints[1], dstPoints[2], dstPoints[3]);
        renderHeight = distance(dstPoints[0], dstPoints[1], dstPoints[4], dstPoints[5]);
    }

    final static float[] srcPoints =
            new float[]{0f /* x0 */, 0f /* y0 */, 1f /* x1 */, 0f /* y1 */, 0f /* x2 */, 1f /* y2 */};
    private final float[] dstPoints = new float[6];
    private final Point renderSize = new Point();
    private int renderWidth;
    private int renderHeight;
    private final Matrix renderMatrix = new Matrix();

    private static int distance(float x0, float y0, float x1, float y1) {
        return (int) Math.round(Math.hypot(x1 - x0, y1 - y0));
    }

    public void drawYuv(int[] yuvTextures,
                        int viewportX, int viewportY, int viewportWidth, int viewportHeight) {
        calculateTransformedRenderSize(0, 0, null);
        renderMatrix.reset();
        /*该套矩阵变换效果为镜像Scale正负值表示是否镜像沿x|y轴镜像*/
        renderMatrix.preTranslate(0.5f, 0.5f);
        renderMatrix.preScale(1f, -1f); // I420-frames are upside down
        renderMatrix.preRotate(0);
        renderMatrix.preTranslate(-0.5f, -0.5f);
        drawYuv(yuvTextures, convertMatrixFromAndroidGraphicsMatrix(renderMatrix), viewportX, viewportY, viewportWidth, viewportHeight);
    }

    /**
     * Draw a YUV frame with specified texture transformation matrix. Required resources are
     * allocated at the first call to this function.
     */
    @Override
    public void drawYuv(int[] yuvTextures, float[] texMatrix,
                        int viewportX, int viewportY, int viewportWidth, int viewportHeight) {


        prepareShader(YUV_FRAGMENT_SHADER_STRING, texMatrix);
        //Bind the textures.
        //后面渲染的时候，设置三成纹理
        for (int i = 0; i < 3; ++i) {
            GLES20.glActiveTexture(GLES20.GL_TEXTURE0 + i);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, yuvTextures[i]);
        }
        drawRectangle(viewportX, viewportY, viewportWidth, viewportHeight);
        // Unbind the textures as a precaution..
        for (int i = 0; i < 3; ++i) {
            GLES20.glActiveTexture(GLES20.GL_TEXTURE0 + i);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, 0);
        }
    }

    private void drawRectangle(int x, int y, int width, int height) {
        // Draw quad.
        // 图形绘制
        GLES20.glViewport(x, y, width, height);
        GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
    }

    private void prepareShader(String fragmentShader, float[] texMatrix) {
        final Shader shader;
        if (shaders.containsKey(fragmentShader)) {
            shader = shaders.get(fragmentShader);
        } else {
            // Lazy allocation.
            shader = new Shader(fragmentShader);
            shaders.put(fragmentShader, shader);
            shader.glShader.useProgram();
            // Initialize fragment shader uniform values.
            if (YUV_FRAGMENT_SHADER_STRING.equals(fragmentShader)) {
                //对这几个纹理采样器变量进行设置
                GLES20.glUniform1i(shader.glShader.getUniformLocation("y_tex"), 0);
                GLES20.glUniform1i(shader.glShader.getUniformLocation("u_tex"), 1);
                GLES20.glUniform1i(shader.glShader.getUniformLocation("v_tex"), 2);
            }
            GlUtil.checkNoGLES2Error("Initialize fragment shader uniform values.");
            // Initialize vertex shader attributes.  设置Vertex Shader数据
            // 顶点位置数据传入着色器
            shader.glShader.setVertexAttribArray("in_pos", 2, FULL_RECTANGLE_BUF);
            shader.glShader.setVertexAttribArray("in_tc", 2, FULL_RECTANGLE_TEX_BUF);
        }
        shader.glShader.useProgram();
        // Copy the texture transformation matrix over.
        // 将最终变换矩阵传入shader程序
        GLES20.glUniformMatrix4fv(shader.texMatrixLocation, 1, false, texMatrix, 0);
    }

    /**
     * Release all GLES resources. This needs to be done manually, otherwise the resources are leaked.
     */
    @Override
    public void release() {
        for (Shader shader : shaders.values()) {
            shader.glShader.release();
        }
        shaders.clear();
    }

    public static float[] convertMatrixFromAndroidGraphicsMatrix(Matrix matrix) {
        float[] values = new float[9];
        matrix.getValues(values);

        // The android.graphics.Matrix looks like this:
        // [x1 y1 w1]
        // [x2 y2 w2]
        // [x3 y3 w3]
        // We want to contruct a matrix that looks like this:
        // [x1 y1  0 w1]
        // [x2 y2  0 w2]
        // [ 0  0  1  0]
        // [x3 y3  0 w3]
        // Since it is stored in column-major order, it looks like this:
        // [x1 x2 0 x3
        //  y1 y2 0 y3
        //   0  0 1  0
        //  w1 w2 0 w3]
        // clang-format off
        float[] matrix4x4 = {
                values[0 * 3 + 0], values[1 * 3 + 0], 0, values[2 * 3 + 0],
                values[0 * 3 + 1], values[1 * 3 + 1], 0, values[2 * 3 + 1],
                0, 0, 1, 0,
                values[0 * 3 + 2], values[1 * 3 + 2], 0, values[2 * 3 + 2],
        };
        // clang-format on
        return matrix4x4;
    }
}
