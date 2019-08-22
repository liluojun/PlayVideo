package sc.playvideo.com.yuvencodedecode.yuv;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class MyGlsurface extends GLSurfaceView {

    private final Glrenderer glrenderer;
    private SurfaceTexture surfaceTexture;
    private boolean isRendererSelf = false;

    public boolean isRendererSelf() {
        return isRendererSelf;
    }

    public void setRendererSelf(boolean rendererSelf) {
        isRendererSelf = rendererSelf;
    }


    public MyGlsurface(Context context) {
        super(context);
        setEGLContextClientVersion(2);//GLContext设置为OpenGLES2.0
        glrenderer = new Glrenderer(this);
        setRenderer(glrenderer);//设置渲染器
        setRenderMode(RENDERMODE_WHEN_DIRTY);
    }

    public MyGlsurface(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(2);
        glrenderer = new Glrenderer(this);
        setRenderer(glrenderer);
        ///*渲染方式，RENDERMODE_WHEN_DIRTY表示被动渲染，只有在调用requestRender或者onResume等方法时才会进行渲染。RENDERMODE_CONTINUOUSLY表示持续渲染*/
        setRenderMode(RENDERMODE_WHEN_DIRTY);
    }

    public SurfaceTexture getSurfaceTexture() {
        return surfaceTexture;
    }

    public SurfaceTexture initSurTexture() {
        int oesTextureObject = GlUtil.createOESTextureObject();
        surfaceTexture = new SurfaceTexture(oesTextureObject);
        surfaceTexture.setOnFrameAvailableListener(new SurfaceTexture.OnFrameAvailableListener() {
            @Override
            public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                setRendererSelf(true);
                requestRender();
            }
        });
        return surfaceTexture;
    }

    public void uplaodTexture(YUVData yuv) {
        glrenderer.upLoadYUV(yuv);
        requestRender();
    }

    public void clearSurfaceTexture() {
        surfaceTexture = null;
    }
}
