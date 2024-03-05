//
// Created by hjt on 2024/2/18.
//

#ifndef PLAYVIDEO_GLDRAW_H
#define PLAYVIDEO_GLDRAW_H

#include "YuvData.h"
#include "GlRendering.h"
#include "EglUtils.cpp"
#include <iostream>
#include <map>

typedef struct Shader {
    GlRendering glShader;
    int texMatrixLocation;
} Shader;

class GlDraw {
public:
    const float FULL_RECTANGLE_BUF[8] = {-1.0f, -1.0f, // Bottom left.
                                         1.0f, -1.0f, // Bottom right.
                                         -1.0f, 1.0f, // Top left.
                                         1.0f, 1.0f, // Top right.
    };

    // Texture coordinates - (0, 0) is bottom-left and (1, 1) is top-right.
    const float FULL_RECTANGLE_TEX_BUF[8] = {
            0.0f, 0.0f, // Bottom left.
            1.0f, 0.0f, // Bottom right.
            0.0f, 1.0f, // Top left.
            1.0f, 1.0f // Top right.
    };

    float matrix4x4[16] = {1.0, 0.0, 0.0, 0.0,
                           -0.0, -1.0, 0.0, -0.0,
                           0.0, 0.0, 1.0, 0.0,
                           0.0, 1.0, 0.0, 1.0};
    std::map<std::string, Shader> shaders;

    void prepareShader(std::string fragmentShader, float *texMatrix);

    void drawYuv(GLuint yuvTextures[3], float *texMatrix,
                 int viewportX, int viewportY, int viewportWidth, int viewportHeight);

    void drawRectangle(int x, int y, int width, int height);

    void release();

    void drawYuv(GLuint yuvTextures[3],
                 int viewportX, int viewportY, int viewportWidth, int viewportHeight);

    void perparDrawYuv(int width, int height, YuvData *data, GLuint yuvTextures[3]);
};


#endif //PLAYVIDEO_GLDRAW_H
