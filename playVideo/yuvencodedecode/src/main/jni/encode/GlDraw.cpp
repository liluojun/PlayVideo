//
// Created by hjt on 2024/2/18.
//

#include "GlDraw.h"

std::string VERTEX_SHADER_STRING =
        "varying vec2 interp_tc;\n"
        "attribute vec4 in_pos;\n"
        "attribute vec4 in_tc;\n"
        "\n"
        "uniform mat4 texMatrix;\n"
        "\n"
        "void main() {\n"
        "    gl_Position = in_pos;\n"
        "    interp_tc = (texMatrix * in_tc).xy;\n"
//        "    interp_tc =  in_tc;\n"
        "}\n";
std::string YUV_FRAGMENT_SHADER_STRING =
        "precision mediump float;\n"
        "varying vec2 interp_tc;\n"
        "\n"
        "uniform sampler2D y_tex;\n"
        "uniform sampler2D u_tex;\n"
        "uniform sampler2D v_tex;\n"
        "\n"
        "void main() {\n"
//        "vec3 yuv;\n"
//        "vec3 rgb;\n"
//        "yuv.x = texture2D(tex_y, texOut).r;\n"
//        "yuv.y = texture2D(tex_u, texOut).r - 0.5;\n"
//        "yuv.z = texture2D(tex_v, texOut).r - 0.5;\n"
//        "rgb = mat3( 1, 1, 1,\n"
//        "0, -0.39465, 2.03211,\n"
//        "1.13983, -0.58060, 0) * yuv;\n"
//        "gl_FragColor = vec4(rgb, 1);\n"
                "  float y = texture2D(y_tex, interp_tc).r;\n"
                "  float u = texture2D(u_tex, interp_tc).r - 0.5;\n"
                "  float v = texture2D(v_tex, interp_tc).r - 0.5;\n"
                "  gl_FragColor = vec4(y + 1.403 * v, "
                "                      y - 0.344 * u - 0.714 * v, "
                "                      y + 1.77 * u, 1);\n"
        "}\n";

int *GlDraw::perparDrawYuv(int width, int height, YuvData *data) {
    int planeWidths[3] = {width, width / 2, width / 2};
    int planeHeights[3] = {height, height / 2, height / 2};
    int yuvTextures[3];
    for (int i = 0; i < 3; i++) {
        yuvTextures[i] = generateTexture(GL_TEXTURE_2D);
    }

    // Upload each plane.
    for (int i = 0; i < 3; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, yuvTextures[i]);
        // GLES only accepts packed data, i.e. stride == planeWidth.
        switch (i) {
            case 0:
                glTexImage2D((GLenum) GL_TEXTURE_2D, (GLint) 0, (GLint) GL_LUMINANCE,
                             (GLsizei) planeWidths[i],
                             (GLsizei) planeHeights[i], (GLint) 0, (GLenum) GL_LUMINANCE,
                             (GLenum) GL_UNSIGNED_BYTE, (GLvoid *) data->y);
                break;
            case 1:
                glTexImage2D((GLenum) GL_TEXTURE_2D, (GLint) 0, (GLint) GL_LUMINANCE,
                             (GLsizei) planeWidths[i],
                             (GLsizei) planeHeights[i], (GLint) 0, (GLenum) GL_LUMINANCE,
                             (GLenum) GL_UNSIGNED_BYTE, (GLvoid *) data->u);
                break;
            case 2:
                glTexImage2D((GLenum) GL_TEXTURE_2D, (GLint) 0, (GLint) GL_LUMINANCE,
                             (GLsizei) planeWidths[i],
                             (GLsizei) planeHeights[i], (GLint) 0, (GLenum) GL_LUMINANCE,
                             (GLenum) GL_UNSIGNED_BYTE, (GLvoid *) data->v);
                break;
        }

    }
    return yuvTextures;
}

void GlDraw::drawYuv(int *yuvTextures,
                     int viewportX, int viewportY, int viewportWidth, int viewportHeight) {
    drawYuv(yuvTextures, matrix4x4, viewportX, viewportY,
            viewportWidth, viewportHeight);
}

/**
 * Draw a YUV frame with specified texture transformation matrix. Required resources are
 * allocated at the first call to this function.
 */

//
void GlDraw::drawYuv(int *yuvTextures, float *texMatrix,
                     int viewportX, int viewportY, int viewportWidth, int viewportHeight) {


    prepareShader(YUV_FRAGMENT_SHADER_STRING, texMatrix);
    //Bind the textures.
    //后面渲染的时候，设置三成纹理
    for (int i = 0; i < 3; ++i) {
        glActiveTexture(
                GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, yuvTextures[i]);
    }
    drawRectangle(viewportX, viewportY, viewportWidth, viewportHeight);
    // Unbind the textures as a precaution..
    for (int i = 0; i < 3; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void GlDraw::drawRectangle(int x, int y, int width, int height) {
    // Draw quad.
    // 图形绘制
    glViewport(x, y, width, height);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void GlDraw::prepareShader(std::string fragmentShader, float *texMatrix) {

    if (shaders.count(fragmentShader) > 0) {
        Shader shader = shaders[fragmentShader];
        shader.glShader.useProgram();
        glUniformMatrix4fv(shader.texMatrixLocation, 1, false, texMatrix);
    } else {
        // Lazy allocation.
        char *s = (char *) VERTEX_SHADER_STRING.data();
        char *s1 = (char *) fragmentShader.c_str();
        GlRendering *mGlRendering = new GlRendering(s, s1);
        int texMatrixLocation = mGlRendering->getUniformLocation("texMatrix");
        Shader *shader = new Shader();
        shader->glShader = *mGlRendering;
        shader->texMatrixLocation = texMatrixLocation;
        shaders[fragmentShader] = *shader;
        shader->glShader.useProgram();
        // Initialize fragment shader uniform values.
        if (YUV_FRAGMENT_SHADER_STRING == fragmentShader) {
            //对这几个纹理采样器变量进行设置
            glUniform1i(shader->glShader.getUniformLocation("y_tex"), 0);
            glUniform1i(shader->glShader.getUniformLocation("u_tex"), 1);
            glUniform1i(shader->glShader.getUniformLocation("v_tex"), 2);
        }
        checkNoGLES2Error("Initialize fragment shader uniform values.");
        // Initialize vertex shader attributes.  设置Vertex Shader数据
        // 顶点位置数据传入着色器
        shader->glShader.setVertexAttribArray("in_pos", 2, 0, FULL_RECTANGLE_BUF);
        checkNoGLES2Error("in_pos.");
        shader->glShader.setVertexAttribArray("in_tc", 2, 0, FULL_RECTANGLE_TEX_BUF);
        checkNoGLES2Error("in_tc.");
        shader->glShader.useProgram();
        glUniformMatrix4fv(shader->texMatrixLocation, 1, false, texMatrix);
    }


}

/**
 * Release all GLES resources. This needs to be done manually, otherwise the resources are leaked.
 */


void GlDraw::release() {
    for (auto[key, val]:shaders) {
        val.glShader.release();

    }
    shaders.clear();
}
