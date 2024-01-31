package com.aliyun.openglc.yuv;

import android.opengl.GLES20;
import android.support.annotation.Nullable;

import java.nio.ByteBuffer;

public class YuvHelper {
    private ByteBuffer copyBuffer;
    private int[] yuvTextures;
    @Nullable
    public int[] getYuvTextures() {
        return yuvTextures;
    }

    public int[] uploadYuvData(int width, int height, ByteBuffer[] planes) {
        final int[] planeWidths = new int[]{width, width / 2, width / 2};
        final int[] planeHeights = new int[]{height, height / 2, height / 2};
        if (yuvTextures == null) {
            yuvTextures = new int[3];
            for (int i = 0; i < 3; i++) {
                yuvTextures[i] = GlUtil.generateTexture(GLES20.GL_TEXTURE_2D);
            }
        }
        // Upload each plane.
        for (int i = 0; i < 3; ++i) {
            GLES20.glActiveTexture(GLES20.GL_TEXTURE0 + i);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, yuvTextures[i]);
            // GLES only accepts packed data, i.e. stride == planeWidth.
            final ByteBuffer packedByteBuffer;
            // Input is packed already.
            packedByteBuffer = planes[i];
            GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_LUMINANCE, planeWidths[i],
                    planeHeights[i], 0, GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE, packedByteBuffer);
        }
        return yuvTextures;
    }
}
