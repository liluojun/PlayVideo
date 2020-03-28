package media.jni;


public class JavaToNativeMethod {
    static JavaToNativeMethod INSTENCE;
    private static NativeMethod nativeMethod;
    private static MediaNative mMediaNative;

    public static JavaToNativeMethod getInstence() {
        if (INSTENCE == null) {
            INSTENCE = new JavaToNativeMethod();
            nativeMethod = new NativeMethod();
            mMediaNative = new MediaNative();
        }
        return INSTENCE;
    }

    public void nv21ToI420(byte[] src, byte[] dst, int w, int h, byte[] y, byte[] u, byte[] v) {
        nativeMethod.nv21ToI420(src, dst, w, h, y, u, v);
    }

    public void nv21ToNv12(byte[] src, byte[] dst, int w, int h, byte[] y, byte[] u, byte[] v) {
        nativeMethod.nv21ToNv12(src, dst, w, h, y, u, v);
    }

    public void cutterYuv(byte[] src, byte[] dst, int w, int h, byte[] y, byte[] u, byte[] v) {
        nativeMethod.nv21ToNv12(src, dst, w, h, y, u, v);
    }

    public void nv21CutterToI420(byte[] src, byte[] dst, int cutter_w, int cutter_h, int w, int h, byte[] y, byte[] u, byte[] v) {
        nativeMethod.nv21CutterToI420(src, dst, cutter_w, cutter_h, w, h, y, u, v);
    }

    public void nv21CutterToNv12(byte[] src, byte[] dst, int cutter_w, int cutter_h, int w, int h, byte[] y, byte[] u, byte[] v) {
        nativeMethod.nv21CutterToNv12(src, dst, cutter_w, cutter_h, w, h, y, u, v);
    }

    public int creatFfmpeg() {
        return mMediaNative.creatFfmpeg();
    }

    public int initContext(int w, int h, int fps) {
        return mMediaNative.initContext(w, h, fps);
    }

    public int encode(byte[] src, byte[] outY, byte[] outV, byte[] outU, int w, int h) {
        return mMediaNative.encode(src, outY, outV, outU, w, h);
    }

    public int openStream(String path) {
        return mMediaNative.openStream(path);
    }

    public void unFfmpeg() {
        mMediaNative.unFfmpeg();
    }

    public void closeStream() {
        mMediaNative.closeStream();
    }
}
