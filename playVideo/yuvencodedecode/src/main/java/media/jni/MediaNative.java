package media.jni;

public class MediaNative {
    /**********************************************/
    public native int creatFfmpeg();

    public native int initContext(int w, int h, int fps);

    public native int encode(byte[] src, byte[] outY, byte[] outV, byte[] outU, int w, int h);

    public native void unFfmpeg();

    /**********************************************/
    public native int openStream(String path);
}
