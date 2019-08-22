package sc.playvideo.com.yuvencodedecode.yuv;

import android.annotation.SuppressLint;
import android.content.Context;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.media.MediaFormat;
import android.util.Log;

import sc.playvideo.com.yuvencodedecode.CameraActivity;
import sc.playvideo.com.yuvencodedecode.Decoder;
import sc.playvideo.com.yuvencodedecode.bean.UiVideoData;

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

    @SuppressLint("NewApi")
    public Encoder(Context context, int width, int height, int framerate, int bitrate, MyGlsurface myGlsurface) {
        try {
            this.context = context;
            colorFormat = selectColorFormat(selectCodec("video/avc"), "video/avc");
            m_width = width;
            m_height = height;
            m_framerate = framerate;
            this.myGlsurface = myGlsurface;
            dst = new byte[m_height * m_width * 3 / 2];
            yByte = new byte[m_height * m_width];
            vByte = new byte[m_height * m_width / 4];
            uByte = new byte[m_height * m_width / 4];
            yuvData = new YUVData();
            yuvData.yuvH = width;
            yuvData.yuvW = height;
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
            m_framerate = framerate;
            this.myGlsurface = myGlsurface;
            dst = new byte[m_height * m_width * 3 / 2];
            yByte = new byte[m_height * m_width];
            vByte = new byte[m_height * m_width / 4];
            uByte = new byte[m_height * m_width / 4];
            yuvData = new YUVData();
            yuvData.yuvH = height;
            yuvData.yuvW = width;
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
                    byte[] yuv420sp = new byte[m_width * m_height * 3 / 2];
                    if (colorFormat <= 20) {
                        yuv420sp = nv21ToI420(input, m_width, m_height);
                    } else {
                        yuv420sp = NV21ToNV12(input, yuv420sp, m_width, m_height);
                    }
                    input = yuv420sp;

                    yuvData.creatBuffer(yByte, uByte, vByte);
                    if (myGlsurface != null) {
                        myGlsurface.uplaodTexture(yuvData);
                    }
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
//                                        if (context instanceof CameraActivity) {
//////                                            CameraActivity cameraActivity = (CameraActivity) context;
//////                                            cameraActivity.updateEncodeData(u);
//////                                        }
                                        Decoder.getDecoder().onFrame(u);
                                    }
                                } else {
                                    u._len = outData.length;
                                    u._data = outData;
                                    //编码后的数据
                                    if (myGlsurface != null) {
//                                        if (context instanceof CameraActivity) {
//                                            CameraActivity cameraActivity = (CameraActivity) context;
//                                            cameraActivity.updateEncodeData(u);
//                                        }
                                        Decoder.getDecoder().onFrame(u);
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

    public byte[] configbyteShare;
    public boolean isKeyExists = false;

    /*
     * 编码并返回数据
     */
    @SuppressLint({"NewApi", "WrongConstant"})
    @SuppressWarnings("deprecation")
    public List<byte[]> offerEncoder(byte[] input) {
        int pos = 0;
        isRuning = true;
        List<byte[]> list = new ArrayList<byte[]>();
        long pts = 0;
        try {
            long startMs = System.currentTimeMillis();
            ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();
            ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();
            int inputBufferIndex = mediaCodec.dequeueInputBuffer(0);
            if (inputBufferIndex >= 0) {
                pts = computePresentationTime(generateIndex);
                ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
                inputBuffer.clear();
                inputBuffer.put(input);
                mediaCodec.queueInputBuffer(inputBufferIndex, 0, input.length, pts, 0);
                generateIndex += 1;
            } else {
                return null;
            }
            int outputBufferIndex = -1;
            try {
                MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
                outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 1200);
                while (outputBufferIndex >= 0) {
                    UiVideoData u = new UiVideoData();
                    ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
                    byte[] outData = new byte[bufferInfo.size];
                    outputBuffer.get(outData);
                    // 获取sps-pps

                    if (configbyteShare == null) {
                        ByteBuffer spsPpsBuffer = ByteBuffer.wrap(outData);
                        if (spsPpsBuffer.getInt() == 0x00000001) {
                            byte[] mMediaHead = new byte[outData.length];
                            System.arraycopy(outData, 0, mMediaHead, 0, outData.length);
                            configbyteShare = outData;
                            mMediaHead = null;
                            outData = null;
                            break;
                        }
                    } else {
                        if ((outData[4] & 0x1f) == 5) {
                            byte[] keyframe = new byte[bufferInfo.size + configbyteShare.length];
                            System.arraycopy(configbyteShare, 0, keyframe, 0, configbyteShare.length);
                            System.arraycopy(outData, 0, keyframe, configbyteShare.length, outData.length);
                            list.add(keyframe);
                            isKeyExists = true;

                        } else {
                            list.add(outData);
                            pos = 10;
                        }

                        mediaCodec.releaseOutputBuffer(outputBufferIndex, false);
                        outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
                    }
                }
            } catch (Exception e) {
                if (outputBufferIndex >= 0) {
                    mediaCodec.releaseOutputBuffer(outputBufferIndex, false);
                }
                // Log.e("Encoder", "encoder-193");
            }


        } catch (Throwable t) {

        }
        return list;
    }

    public void swapYV12toI420(byte[] yv12bytes, byte[] i420bytes, int width, int height) {
        System.arraycopy(yv12bytes, 0, i420bytes, 0, width * height);
        System.arraycopy(yv12bytes, width * height + width * height / 4, i420bytes, width * height, width * height / 4);
        System.arraycopy(yv12bytes, width * height, i420bytes, width * height + width * height / 4, width * height / 4);
    }

    public byte[] nv21ToI420(byte[] data, int width, int height) {
        //dst = data;
        YUV420spRotate270(dst, data, width, height);
        byte[] ret = new byte[width * height * 3 / 2];
        int total = width * height;

        ByteBuffer bufferY = ByteBuffer.wrap(ret, 0, total);
        ByteBuffer bufferU = ByteBuffer.wrap(ret, total, total / 4);
        ByteBuffer bufferV = ByteBuffer.wrap(ret, total + total / 4, total / 4);

        bufferY.put(dst, 0, total);
        System.arraycopy(dst, 0, yByte, 0, total);
        for (int i = total; i < data.length; i += 2) {
            bufferV.put(dst[i]);
            bufferU.put(dst[i + 1]);
            uByte[(i - total) / 2] = dst[i + 1];
            vByte[(i - total) / 2] = dst[i];
        }

        return ret;
    }

    boolean b1 = true;

    public byte[] NV21_2_yuv420p(byte[] dst, byte[] src, int w, int h) {

        int ysize = w * h;
        int usize = ysize / 4;
        // y
        System.arraycopy(src, 0, dst, 0, ysize);
        // v
        for (int i = 0; i < usize; i++) {
            dst[ysize + usize + i] = src[ysize + 2 * i];//
        }
        // u
        for (int i = 0; i < usize; i++) {
            dst[ysize + i] = src[ysize + 2 * i + 1];// 307200//307202//307204//307206
            // 307200//307201//307202//307203
        }

        return dst;
    }

    public byte[] NV21_2_yuv420sp(byte[] dst, byte[] src, int w, int h) {

        int ysize = w * h;
        int usize = ysize / 2;
        // y
        System.arraycopy(src, 0, dst, 0, ysize);
        // v
        for (int i = 0; i < usize; i += 2) {
            dst[ysize + i] = src[ysize + i + 1];//
            dst[ysize + i + 1] = src[ysize + i];
        }
        return dst;
    }

    private byte[] intToByteArray(final int integer) {
        int byteNum = (40 - Integer.numberOfLeadingZeros(integer < 0 ? ~integer : integer)) / 8;
        byte[] byteArray = new byte[4];

        for (int n = 0; n < byteNum; n++)
            byteArray[3 - n] = (byte) (integer >>> (n * 8));

        return (byteArray);
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

    public byte[] NV21ToNV12(byte[] nv21, byte[] nv12, int width, int height) {
        try {
            if (nv21 == null || nv12 == null)
                return null;
            int framesize = width * height;
            YUV420spRotate270(dst, nv21, width, height);
            for (int i = 0; i < framesize; i++) {
                nv12[i] = dst[i];
                yByte[i] = dst[i];
            }
            for (int j = 0; j < framesize / 2; j += 2) {
                nv12[framesize + j - 1] = dst[j + framesize];
                vByte[j / 2] = dst[j + framesize];
            }
            for (int j = 0; j < framesize / 2; j += 2) {
                nv12[framesize + j] = dst[j + framesize - 1];
                uByte[j / 2] = dst[j + framesize - 1];
            }
            return nv12;
        } catch (Exception e) {
            return null;

        }
    }

    /**
     * @param data
     * @param imageWidth
     * @param imageHeight
     * @return
     */
    public byte[] rotateYUV420Degree90(byte[] data, int imageWidth, int imageHeight) {
        byte[] yuv = new byte[imageWidth * imageHeight * 3 / 2];
        // Rotate the Y luma
        int i = 0;
        for (int x = 0; x < imageWidth; x++) {
            for (int y = imageHeight - 1; y >= 0; y--) {
                yuv[i] = data[y * imageWidth + x];
                i++;
            }

        }
        // Rotate the U and V color components
        i = imageWidth * imageHeight * 3 / 2 - 1;
        for (int x = imageWidth - 1; x > 0; x = x - 2) {
            for (int y = 0; y < imageHeight / 2; y++) {
                yuv[i] = data[(imageWidth * imageHeight) + (y * imageWidth) + x];
                i--;
                yuv[i] = data[(imageWidth * imageHeight) + (y * imageWidth) + (x - 1)];
                i--;
            }
        }
        return yuv;
    }

    private void YUV420spRotate270(byte[] des, byte[] src, int width, int height) {
        if (isRotate) {
            int n = 0;
            int uvHeight = height >> 1;
            int wh = width * height;
            //copy y
            for (int j = width - 1; j >= 0; j--) {
                for (int i = 0; i < height; i++) {
                    des[n++] = src[width * i + j];
                }
            }

            for (int j = width - 1; j > 0; j -= 2) {
                for (int i = 0; i < uvHeight; i++) {
                    des[n++] = src[wh + width * i + j - 1];
                    des[n++] = src[wh + width * i + j];
                }
            }
        } else {
            dst = src;
        }
    }


    private byte[] rotateYUV420Degree270(byte[] data, int imageWidth, int imageHeight) {
        byte[] yuv = new byte[imageWidth * imageHeight * 3 / 2];
        // Rotate the Y luma
        int i = 0;
        for (int x = imageWidth - 1; x >= 0; x--) {
            for (int y = 0; y < imageHeight; y++) {
                yuv[i] = data[y * imageWidth + x];
                i++;
            }
        }// Rotate the U and V color components
        i = imageWidth * imageHeight;
        for (int x = imageWidth - 1; x > 0; x = x - 2) {
            for (int y = 0; y < imageHeight / 2; y++) {
                yuv[i] = data[(imageWidth * imageHeight) + (y * imageWidth) + (x - 1)];
                i++;
                yuv[i] = data[(imageWidth * imageHeight) + (y * imageWidth) + x];
                i++;
            }
        }
        return yuv;
    }

    void swapNV12toI420(byte[] nv12bytes, byte[] i420bytes, int width, int height) {

        int nLenY = width * height;
        int nLenU = nLenY / 4;

        System.arraycopy(nv12bytes, 0, i420bytes, 0, width * height);
        for (int i = 0; i < nLenU; i++) {
            i420bytes[nLenY + i] = nv12bytes[nLenY + 2 * i + 1];
            i420bytes[nLenY + nLenU + i] = nv12bytes[nLenY + 2 * i];
        }
    }

    /**
     * Generates the presentation time for frame N, in microseconds.
     */
    private long computePresentationTime(long frameIndex) {
        return 132 + frameIndex * 1000000 / m_framerate;
    }

    public void encodeYUV420SP(byte[] yuv420sp, byte[] argb, int width, int height) {
        final int frameSize = width * height;

        int yIndex = 0;
        int uvIndex = frameSize;

        int a, R, G, B, Y, U, V;
        int index = 0;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {

                a = (argb[index] & 0xff000000) >> 24; // a is not used obviously
                R = (argb[index] & 0xff0000) >> 16;
                G = (argb[index] & 0xff00) >> 8;
                B = (argb[index] & 0xff) >> 0;

                // well known RGB to YUV algorithm
                Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
                U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
                V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

                // NV21 has a plane of Y and interleaved planes of VU each
                // sampled by a factor of 2
                // meaning for every 4 Y pixels there are 1 V and 1 U. Note the
                // sampling is every other
                // pixel AND every other scanline.
                yuv420sp[yIndex++] = (byte) ((Y < 0) ? 0 : ((Y > 255) ? 255 : Y));
                if (j % 2 == 0 && index % 2 == 0) {
                    yuv420sp[uvIndex++] = (byte) ((V < 0) ? 0 : ((V > 255) ? 255 : V));
                    yuv420sp[uvIndex++] = (byte) ((U < 0) ? 0 : ((U > 255) ? 255 : U));
                }

                index++;
            }
        }
    }
}
