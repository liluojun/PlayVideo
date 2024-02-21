package media.jni;

import android.view.Surface;

public class MediaNative {
    /**********************************************/
    public native int creatFfmpeg();

    public native int initContext(int w, int h, int fps);

    public native int encode(byte[] src, byte[] outY, byte[] outV, byte[] outU, int w, int h);

    public native void unFfmpeg();

    public native void closeStream();

    public native int initRender();

    public native long creatSurface(Surface surface, int w, int h);

    public native int destorySurface(long l);

    public native int changeSurfaceSize(long l,int w, int h);

    /**********************************************/
    public native int openStream(String path);
}
