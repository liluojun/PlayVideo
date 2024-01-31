package com.media.opengl;

public class NativeMethod {
    public native void nv21ToI420(byte[] src, byte[] dst, int w, int h, byte[] y, byte[] u, byte[] v);

    public native void nv21ToNv12(byte[] src, byte[] dst, int w, int h, byte[] y, byte[] u, byte[] v);

    public native void nv21CutterToI420(byte[] src, byte[] dst, int cutter_w, int cutter_h, int w, int h, byte[] y, byte[] u, byte[] v);

    public native void nv21CutterToNv12(byte[] src, byte[] dst, int cutter_w, int cutter_h, int w, int h, byte[] y, byte[] u, byte[] v);
}
