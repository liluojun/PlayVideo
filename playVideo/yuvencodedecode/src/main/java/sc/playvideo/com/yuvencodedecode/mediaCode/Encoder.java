package sc.playvideo.com.yuvencodedecode.mediaCode;

import android.annotation.SuppressLint;
import android.content.Context;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.media.MediaFormat;
import android.os.Environment;
import android.util.Log;

import media.jni.JavaToNativeMethod;
import sc.playvideo.com.yuvencodedecode.mediaCode.Decoder;
import sc.playvideo.com.yuvencodedecode.bean.UiVideoData;
import sc.playvideo.com.yuvencodedecode.yuv.MyGlsurface;
import sc.playvideo.com.yuvencodedecode.yuv.YUVData;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

public class Encoder {
    private final static String TAG = "MeidaCodec";

    private int TIMEOUT_USEC = 12000;

    private MediaCodec mediaCodec;
    int m_width;
    int m_height;
    int m_framerate;

    public byte[] configbyte;
    public int colorFormat;
    public byte[] yByte;
    public byte[] uByte;
    public byte[] vByte;
    UiVideoData uvd;
    private byte[] dst;
    private YUVData yuvData;
    boolean isRotate = true;
    private MyGlsurface myGlsurface;

    private Context context;
    private byte[] yuv420sp;

    @SuppressLint("NewApi")
    public Encoder(Context context, int width, int height, int framerate, int bitrate, MyGlsurface myGlsurface) {
        try {
            this.context = context;
            colorFormat = selectColorFormat(selectCodec("video/avc"), "video/avc");
            m_width = width;
            m_height = height;
            yuv420sp = new byte[m_width * m_height * 3 / 2];
            m_framerate = framerate;
            this.myGlsurface = myGlsurface;
            dst = new byte[m_height * m_width * 3 / 2];
            yByte = new byte[m_height * m_width];
            vByte = new byte[m_height * m_width / 4];
            uByte = new byte[m_height * m_width / 4];
            yuvData = new YUVData();
            yuvData.yuvH = height;
            yuvData.yuvW = width;
            MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
            mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, colorFormat);
            mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, bitrate);
            mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, m_framerate);
            mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 2);
            try {
                mediaCodec = MediaCodec.createEncoderByType("video/avc");
            } catch (Exception e) {
                e.printStackTrace();
            }
            mediaCodec.configure(mediaFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
            mediaCodec.start();

            uvd = new UiVideoData();

        } catch (Exception e) {
            Log.e("Encoder", "Encoder-64");
            e.printStackTrace();
        }
    }

    @SuppressLint("NewApi")
    public Encoder(Context context, int width, int height, int framerate, int bitrate, boolean noReta, MyGlsurface myGlsurface) {
        try {
            this.context = context;
            colorFormat = selectColorFormat(selectCodec("video/avc"), "video/avc");
            m_width = width;
            m_height = height;
            yuv420sp = new byte[m_width * m_height * 3 / 2];
            m_framerate = framerate;
            this.myGlsurface = myGlsurface;
            dst = new byte[m_height * m_width * 3 / 2];
            yByte = new byte[m_height * m_width];
            vByte = new byte[m_height * m_width / 4];
            uByte = new byte[m_height * m_width / 4];
            yuvData = new YUVData();
            yuvData.yuvH = width;
            yuvData.yuvW = height;
            isRotate = noReta;
            MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", width, height);
            mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, colorFormat);
            mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, bitrate);
            mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, m_framerate);
            mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 2);
            try {
                mediaCodec = MediaCodec.createEncoderByType("video/avc");
            } catch (Exception e) {
                e.printStackTrace();
            }
            mediaCodec.configure(mediaFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
            mediaCodec.start();

            uvd = new UiVideoData();
        } catch (Exception e) {
            Log.e("Encoder", "Encoder-64");
            e.printStackTrace();
        }
    }

    public void release() {
        if (mediaCodec != null) {
            mediaCodec.stop();
            mediaCodec.release();
        }

    }

    void wfile(byte[] b) {
        try {
            File file = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/aa.yuv");
            if (!file.exists())
                file.createNewFile();

            FileOutputStream os = new FileOutputStream(file);
            os.write(b);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @SuppressLint("NewApi")
    public void StopEncoder() {
        try {
            mediaCodec.stop();
            mediaCodec.release();
        } catch (Exception e) {
            // Log.e("encoder", "关闭编码器错误");
            e.printStackTrace();
        }
    }

    public void reStartEncoder() {
        try {
            mediaCodec.stop();
            mediaCodec.start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public boolean isRuning = true;

    public void StopThread() {
        isRuning = false;
    }

    private byte[] mMediaHead = null;
    long generateIndex = 0;

    /**
     * 编码
     *
     * @param input
     * @param output
     * @return
     */
    long pts = 0;

    private int count = 0;
    boolean encode = true;

    public void setEncode(boolean encode) {
        this.encode = encode;
    }

    @SuppressLint("NewApi")
    @SuppressWarnings("deprecation")
    public void encoder(byte[] input) {
        if (isRuning) {
            try {
                if (input != null && input.length != 0) {
                    //**由于Android 摄像头默认采集的数据是NV21格式,所以要
                    //转成MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar
                    //让编码器支持
                    if (colorFormat <= 20) {
                        JavaToNativeMethod.getInstence().nv21ToI420(input, yuv420sp, m_width, m_height, yByte, uByte, vByte);
                    } else {
                        long l = System.currentTimeMillis();
                        JavaToNativeMethod.getInstence().nv21ToNv12(input, yuv420sp, m_width, m_height, yByte, uByte, vByte);
                        Log.e(TAG, "encoder time=" + (System.currentTimeMillis() - l));

                    }
                    yuvData.creatBuffer(yByte, uByte, vByte);
                    if (myGlsurface != null) {
                        myGlsurface.uplaodTexture(yuvData);
                    }

                    input = yuv420sp;
                }
                if (!encode) {
                    return;
                }
                if (input != null && input.length != 0) {
                    try {
                        ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();
                        ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();
                        int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);
                        Log.e("index", "" + inputBufferIndex);
                        if (inputBufferIndex >= 0) {
                            pts = computePresentationTime(generateIndex);
                            ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
                            inputBuffer.clear();
                            inputBuffer.put(input);
                            /**此处generateIndex维护一个递增的时间戳
                             据说此处的第四个参数不传,第一个I帧以后,
                             mediaCodec.dequeueOutputBuffer()一直返回-1,
                             -1对应原生代码的再试一遍的意思*/
                            mediaCodec.queueInputBuffer(inputBufferIndex, 0, input.length, pts, 0);
                            generateIndex += 1;
                        } else {
                            return;
                        }
                        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
                        int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, TIMEOUT_USEC);
                        while (outputBufferIndex >= 0) {
                            UiVideoData u = new UiVideoData();
                            ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
                            byte[] outData = new byte[bufferInfo.size];
                            outputBuffer.get(outData);
                            if (flush)
                                Log.e("bufferInfo", bufferInfo.size + "");
                            // 获取sps pps
                            if (configbyte == null) {
                                ByteBuffer spsPpsBuffer = ByteBuffer.wrap(outData);
                                if (spsPpsBuffer.getInt() == 0x00000001) {
                                    byte[] mMediaHead = new byte[outData.length];
                                    System.arraycopy(outData, 0, mMediaHead, 0, outData.length);
                                    configbyte = outData;
                                    Decoder.getDecoder().Setspspps(outData);
                                    Decoder.getDecoder().creatMediaCode(m_width, m_height);
                                    mMediaHead = null;
                                    outData = null;
                                    break;
                                }

                            } else {

                                if ((outData[4] & 0x1f) == 5) {
                                    byte[] keyframe = new byte[bufferInfo.size + configbyte.length];
                                    System.arraycopy(configbyte, 0, keyframe, 0, configbyte.length);
                                    System.arraycopy(outData, 0, keyframe, configbyte.length, outData.length);

                                    u._len = keyframe.length;
                                    u._data = keyframe;
                                    //编码后的数据
                                    if (myGlsurface != null) {
                                        //  Decoder.getDecoder().onFrame(u);
                                        Decoder.getDecoder().onFFmpegFrame(u);
                                    }
                                } else {
                                    u._len = outData.length;
                                    u._data = outData;
                                    //编码后的数据
                                    if (myGlsurface != null) {
                                        //  Decoder.getDecoder().onFrame(u);
                                        Decoder.getDecoder().onFFmpegFrame(u);
                                    }
                                }
                                mediaCodec.releaseOutputBuffer(outputBufferIndex, false);
                                outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, TIMEOUT_USEC);
                            }
                            outData = null;
                            u = null;
                        }
                        bufferInfo = null;
                    } catch (Exception e) {
                        Log.e("Encoder", "编码错误" + e.getMessage());
                    }
                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    boolean a = false;

    public void encoder(byte[] input, int w, int h) {
        if (isRuning) {
            try {
                if (input != null && input.length != 0) {
                    if (colorFormat <= 20) {
                        JavaToNativeMethod.getInstence().nv21CutterToI420(input, yuv420sp, m_width, m_height, w, h, yByte, uByte, vByte);
                    } else {
                        long l = System.currentTimeMillis();
                        JavaToNativeMethod.getInstence().nv21CutterToNv12(input, yuv420sp, m_width, m_height, w, h, yByte, uByte, vByte);
                        Log.e(TAG, "encoder time=" + (System.currentTimeMillis() - l));
                    }

                    input = yuv420sp;
                    yuvData.creatBuffer(yByte, uByte, vByte);
                    if (myGlsurface != null) {
                        myGlsurface.uplaodTexture(yuvData);
                    }
                    //  Log.e(TAG,"encoder input="+input.length+"**w"+w+"**h"+h+"***m_width"+m_width+"***m_height="+m_height);
                }
                if (!encode) {
                    return;
                }
                if (input != null && input.length != 0) {
                    try {
                        ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();
                        ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();
                        int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);
                        if (inputBufferIndex >= 0) {
                            pts = computePresentationTime(generateIndex);
                            ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
                            inputBuffer.clear();
                            inputBuffer.put(input);
                            mediaCodec.queueInputBuffer(inputBufferIndex, 0, input.length, pts, 0);
                            generateIndex += 1;
                        } else {
                            return;
                        }
                        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
                        int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, TIMEOUT_USEC);
                        while (outputBufferIndex >= 0) {
                            UiVideoData u = new UiVideoData();
                            ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
                            byte[] outData = new byte[bufferInfo.size];
                            outputBuffer.get(outData);
                            // 获取sps pps
                            if (configbyte == null) {
                                ByteBuffer spsPpsBuffer = ByteBuffer.wrap(outData);
                                if (spsPpsBuffer.getInt() == 0x00000001) {
                                    byte[] mMediaHead = new byte[outData.length];
                                    System.arraycopy(outData, 0, mMediaHead, 0, outData.length);
                                    configbyte = outData;
                                    Decoder.getDecoder().Setspspps(outData);
                                    Decoder.getDecoder().creatMediaCode(m_width, m_height);
                                    mMediaHead = null;
                                    outData = null;
                                    break;
                                }

                            } else {
                                if ((outData[4] & 0x1f) == 5) {
                                    byte[] keyframe = new byte[bufferInfo.size + configbyte.length];
                                    System.arraycopy(configbyte, 0, keyframe, 0, configbyte.length);
                                    System.arraycopy(outData, 0, keyframe, configbyte.length, outData.length);

                                    u._len = keyframe.length;
                                    u._data = keyframe;
                                    //编码后的数据
                                    if (myGlsurface != null) {
                                        // Decoder.getDecoder().onFrame(u);
                                        Decoder.getDecoder().onFFmpegFrame(u);
                                    }
                                } else {
                                    u._len = outData.length;
                                    u._data = outData;
                                    //编码后的数据
                                    if (myGlsurface != null) {
                                        // Decoder.getDecoder().onFrame(u);
                                        Decoder.getDecoder().onFFmpegFrame(u);
                                    }
                                }
                                mediaCodec.releaseOutputBuffer(outputBufferIndex, false);
                                outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, TIMEOUT_USEC);
                            }
                            outData = null;
                            u = null;
                        }
                        bufferInfo = null;
                    } catch (Exception e) {
                    }
                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    boolean flush = false;

    public void flush() {
        if (mediaCodec != null) {
            flush = true;
            mediaCodec.flush();
        }
    }


    /**
     * Returns the first codec capable of encoding the specified MIME type, or null if no match was found.
     */
    @SuppressLint("NewApi")
    private static MediaCodecInfo selectCodec(String mimeType) {
        int numCodecs = MediaCodecList.getCodecCount();
        for (int i = 0; i < numCodecs; i++) {
            MediaCodecInfo codecInfo = MediaCodecList.getCodecInfoAt(i);

            if (!codecInfo.isEncoder()) {
                continue;
            }

            String[] types = codecInfo.getSupportedTypes();
            for (int j = 0; j < types.length; j++) {
                if (types[j].equalsIgnoreCase(mimeType)) {
                    return codecInfo;
                }
            }
        }
        return null;
    }

    /**
     * Returns a color format that is supported by the codec and by this test code. If no match is found, this throws a test failure -- the set of formats known to the test should be expanded for new platforms.
     */
    @SuppressLint("NewApi")
    private static int selectColorFormat(MediaCodecInfo codecInfo, String mimeType) {
        MediaCodecInfo.CodecCapabilities capabilities = codecInfo.getCapabilitiesForType(mimeType);
        for (int i = 0; i < capabilities.colorFormats.length; i++) {
            int colorFormat = capabilities.colorFormats[i];
            if (isRecognizedFormat(colorFormat)) {
                return colorFormat;
            }
        }
        return 0; // not reached
    }

    private static boolean isRecognizedFormat(int colorFormat) {
        switch (colorFormat) {
            // these are the formats we know how to handle for this test
            case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar:
            case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedPlanar:
            case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar:
            case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedSemiPlanar:
            case MediaCodecInfo.CodecCapabilities.COLOR_TI_FormatYUV420PackedSemiPlanar:
                return true;
            default:
                return false;
        }
    }

    /**
     * Generates the presentation time for frame N, in microseconds.
     */
    private long computePresentationTime(long frameIndex) {
        return 132 + frameIndex * 1000000 / m_framerate;
    }


}
