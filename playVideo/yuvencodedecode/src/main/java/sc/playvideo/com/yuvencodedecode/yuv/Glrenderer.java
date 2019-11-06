package sc.playvideo.com.yuvencodedecode.yuv;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

import java.nio.ByteBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class Glrenderer implements GLSurfaceView.Renderer {
    private GlRectDrawer mDrawer = new GlRectDrawer();
    private YuvHelper mYuvHelper = new YuvHelper();
    private int surfaceW = 0;
    private int surfaceH = 0;
    YUVData yuvdata;
    MyGlsurface myGlsurface;

    public Glrenderer(MyGlsurface myGlsurface) {
        this.myGlsurface = myGlsurface;
    }

    public void upLoadYUV(YUVData data) {
        if (data != null && !data.isNull()) {
            yuvdata = data;
        }
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//视频栏的背景色 黑色
        //设置颜色缓存的清除值
        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
        //设置背景的颜色
//        GLES20.glClearColor(1.0f, 0.0f, 0.0f, 1.0f);//红色
//        GLES20.glClearColor(29.0f, 29.0f, 31.0f, 1.0f);//视频栏的背景色  白色

    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int j) {
        surfaceH = j;
        surfaceW = i;
        GLES20.glViewport(0, 0, i, j);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        if (myGlsurface.getSurfaceTexture() != null && myGlsurface.isRendererSelf()) {
            myGlsurface.setRendererSelf(false);
            myGlsurface.getSurfaceTexture().updateTexImage();

        }
        if (yuvdata != null) {
            mYuvHelper.uploadYuvData(yuvdata.yuvW, yuvdata.yuvH, new ByteBuffer[]{yuvdata.y, yuvdata.u, yuvdata.v});
            mDrawer.drawYuv(mYuvHelper.getYuvTextures(), 0, 0, surfaceW, surfaceH);
        }
    }

}
