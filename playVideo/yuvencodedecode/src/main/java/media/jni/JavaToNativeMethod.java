package media.jni;


public class JavaToNativeMethod {
    static JavaToNativeMethod INSTENCE;
    private static NativeMethod nativeMethod;

    public static JavaToNativeMethod getInstence() {
        if (INSTENCE == null) {
            INSTENCE = new JavaToNativeMethod();
            nativeMethod = new NativeMethod();
        }
        return INSTENCE;
    }

    public void nv21ToI420(byte[] src, byte[] dst, int w, int h) {
        nativeMethod.nv21ToI420(src, dst, w, h);
    }

    public void nv21ToNv12(byte[] src, byte[] dst, int w, int h) {
        nativeMethod.nv21ToNv12(src, dst, w, h);
    }

    public void cutterYuv(byte[] src, byte[] dst, int w, int h) {
        nativeMethod.nv21ToNv12(src, dst, w, h);
    }

    public void nv21CutterToI420(byte[] src, byte[] dst, int cutter_w, int cutter_h, int w, int h) {
        nativeMethod.nv21CutterToI420(src, dst, cutter_w, cutter_h, w, h);
    }

    public void nv21CutterToNv12(byte[] src, byte[] dst, int cutter_w, int cutter_h, int w, int h) {
        nativeMethod.nv21CutterToNv12(src, dst, cutter_w, cutter_h, w, h);
    }
}
