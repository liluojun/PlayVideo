package media.jni;

public class NativeMethod {
    public native void nv21ToI420(byte[]src,byte[]dst,int w,int h);
    public native void nv21ToNv12(byte[]src,byte[]dst,int w,int h);

}
