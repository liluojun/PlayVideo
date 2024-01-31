package com.aliyun.openglc.yuv;


import java.nio.ByteBuffer;

public class YUVData {
    public int id;
    public int yuvW;
    public int yuvH;
    public ByteBuffer y;
    public ByteBuffer u;
    public ByteBuffer v;

    public boolean isNull() {
        if (y == null || u == null || v == null) {
            return true;
        }
        return false;
    }

    public void creatBuffer(byte[] by, byte[] bu, byte[] bv) {
        y = ByteBuffer.wrap(by);
        u = ByteBuffer.wrap(bu);
        v = ByteBuffer.wrap(bv);
    }

}
