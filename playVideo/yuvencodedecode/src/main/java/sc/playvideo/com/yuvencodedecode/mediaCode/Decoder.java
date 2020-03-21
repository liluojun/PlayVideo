package sc.playvideo.com.yuvencodedecode.mediaCode;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.util.Log;

import media.jni.JavaToNativeMethod;
import sc.playvideo.com.yuvencodedecode.CameraActivity;
import sc.playvideo.com.yuvencodedecode.bean.UiVideoData;

import java.io.IOException;
import java.nio.ByteBuffer;

public class Decoder {
    static final String TAG = "Decoder";
    private MediaCodec mCodec;
    private MediaFormat mediaformat;
    int m_framerate = 20;
    private int generateIndex;
    CameraActivity cameraActivity;
    int w, h;
    private byte[] yByte;
    private byte[] vByte;
    private byte[] uByte;

    public static Decoder getDecoder() {
        if (decoder == null)
            decoder = new Decoder();
        return decoder;
    }

    private static Decoder decoder = null;
    byte[] sps = null;
    byte[] pps = null;

    public void Setspspps(byte[] spspps) {
        Log.e(TAG, "spspps=" + spspps.toString());
        int indexPPS = -1;
        for (int i = 1; i < spspps.length - 4; i++) {
            if (spspps[i] == 0 && spspps[i + 1] == 0 && spspps[i + 2] == 0 && spspps[i + 3] == 1) {
                indexPPS = i;
            }
        }
        sps = new byte[indexPPS];
        pps = new byte[spspps.length - indexPPS];
        System.arraycopy(spspps, 0, sps, 0, indexPPS);
        System.arraycopy(spspps, indexPPS, pps, 0, pps.length);
    }

    public boolean creatMediaCode(int w, int h) {
        if (sps == null || pps == null) {
            return false;
        }
        try {
            mCodec = MediaCodec.createDecoderByType("video/avc");
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        this.w = w;
        this.h = h;
        yByte = new byte[w * h];
        vByte = new byte[w * h / 4];
        uByte = new byte[w * h / 4];
        mediaformat = MediaFormat.createVideoFormat("video/avc", w, h);
        mediaformat.setByteBuffer("csd-0", ByteBuffer.wrap(sps));
        mediaformat.setByteBuffer("csd-1", ByteBuffer.wrap(pps));
        mediaformat.setInteger(MediaFormat.KEY_FRAME_RATE, m_framerate);
        mediaformat.setInteger(MediaFormat.KEY_BIT_RATE, 1024 * 1000);
        mediaformat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 2);
        mCodec.configure(mediaformat, null, null, 0);
        mCodec.start();
        Log.e(TAG, "creatMediaCode=" + w + "**" + h);
        return true;
    }

    public void onFFmpegFrame(UiVideoData uiVideoData) {
        int encode = JavaToNativeMethod.getInstence().encode(uiVideoData._data, yByte, vByte, uByte, w, h);
        Log.e(TAG, "encode=" + encode);
        if (encode == 0) {
            cameraActivity.shared(yByte, uByte, vByte, w, h);
        }
    }

    public void onFrame(UiVideoData uiVideoData) {
        byte[] buf = uiVideoData._data;
        Log.e(TAG, "buf=" + buf.length);
        ByteBuffer[] inputBuffers = mCodec.getInputBuffers();//MediaCodec在此ByteBuffer[]中获取输入数据
        ByteBuffer[] outputBuffers = mCodec.getOutputBuffers(); // 解码后的数据
        int inputBufferIndex = mCodec.dequeueInputBuffer(100);//获取输入缓冲区的索引
        //Log.e(TAG, "inputBufferIndex=" + inputBufferIndex);
        if (inputBufferIndex >= 0) {
            long pts = computePresentationTime(generateIndex);
            ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
            inputBuffer.clear();
            inputBuffer.put(buf);//先获取缓冲区，再放入值
            mCodec.queueInputBuffer(inputBufferIndex, 0, buf.length, pts, 0);//四个参数，第一个是输入缓冲区的索引，第二个是放入的数据大小，第三个是时间戳，保证递增就是
            generateIndex += 1;
        } else {
            return;
        }
        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();//用于描述解码得到的byte[]数据的相关信息
        int outputBufferIndex = mCodec.dequeueOutputBuffer(bufferInfo, 1000);//-1代表一直等待，0表示不等待
        Log.e(TAG, "outputBufferIndex=" + outputBufferIndex);
        while (outputBufferIndex >= 0) {//大于等于0表示解码器有数据输出
            ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
            byte[] outData = new byte[bufferInfo.size];
            outputBuffer.get(outData);//将Buffer内的数据取出到字节数组中
            cameraActivity.shared(outData, w, h);
            mCodec.releaseOutputBuffer(outputBufferIndex, true);
            outputBufferIndex = -1;
            Log.e(TAG, outData.length + "**while");
        }
    }

    private long computePresentationTime(long frameIndex) {
        return 132 + frameIndex * 1000000 / m_framerate;
    }

    public void setContext(CameraActivity cameraActivity) {
        this.cameraActivity = cameraActivity;
    }

    public void stopDecoder() {
        try {
            mCodec.stop();
            mCodec.release();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
