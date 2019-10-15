package sc.playvideo.com.yuvencodedecode;

import android.content.Context;
import android.content.Intent;
import android.hardware.Camera;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import sc.playvideo.com.yuvencodedecode.bean.UiVideoData;
import sc.playvideo.com.yuvencodedecode.mediaCode.Decoder;
import sc.playvideo.com.yuvencodedecode.yuv.MyGlsurface;
import sc.playvideo.com.yuvencodedecode.yuv.YUVData;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;

public class CameraActivity extends AppCompatActivity {

    private static final String TAG = "CameraActivity";
    private CameraSurfaceManage cameraSurfaceManage;
    private MyGlsurface myGlSurface, myGlSurface1;

    private MediaCodec mediaCodec = null;
    private Button bt;

    public static void start(Context context) {
        Intent intent = new Intent();
        intent.setClass(context, CameraActivity.class);
        context.startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera);

        myGlSurface = findViewById(R.id.myGlSurface);
        myGlSurface1 = findViewById(R.id.myGlSurface1);
        bt = findViewById(R.id.bt);
        bt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cameraSurfaceManage.flush();
            }
        });
       Button bt2 = findViewById(R.id.bt2);
        bt2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                cameraSurfaceManage.flush2();
            }
        });
        cameraSurfaceManage = new CameraSurfaceManage(this, myGlSurface);
        cameraSurfaceManage.initCamera(Camera.CameraInfo.CAMERA_FACING_FRONT);
        Decoder.getDecoder().setContext(this);
    }

    @Override
    protected void onDestroy() {
        if (cameraSurfaceManage != null)
            cameraSurfaceManage.stopCamera();
        super.onDestroy();
    }

    /**
     * h264编码后的数据
     *
     * @param uiVideoData
     */
    public void updateEncodeData(UiVideoData uiVideoData) {
        if (cameraSurfaceManage.getCamera() != null) {
            Camera camera = cameraSurfaceManage.getCamera();
            int w = camera.getParameters().getPreviewSize().width;
            int h = camera.getParameters().getPreviewSize().height;

            MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", w, h);

            try {
                mediaCodec = MediaCodec.createDecoderByType("video/avc");//这里是建立的解码器
            } catch (Exception e) {
                e.printStackTrace();
            }

            mediaCodec.configure(mediaFormat, null, null, 0);
            mediaCodec.start();

            onFrame(uiVideoData, w, h);
//            offerDecoder(uiVideoData);
        }
    }

    private int mCount;

    /**
     * 解码
     */
    public void onFrame(UiVideoData uiVideoData, int w, int h) {
        byte[] buf = uiVideoData._data;

        ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();//MediaCodec在此ByteBuffer[]中获取输入数据
        ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers(); // 解码后的数据
        int inputBufferIndex = mediaCodec.dequeueInputBuffer(100);//获取输入缓冲区的索引

        if (inputBufferIndex >= 0) {
            ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
            inputBuffer.clear();
            inputBuffer.put(buf);//先获取缓冲区，再放入值

            long pts = computePresentationTime(mCount);

            mediaCodec.queueInputBuffer(inputBufferIndex, 0, buf.length, pts, 0);//四个参数，第一个是输入缓冲区的索引，第二个是放入的数据大小，第三个是时间戳，保证递增就是
            mCount++;
        } else {
            return;
        }

        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();//用于描述解码得到的byte[]数据的相关信息
        int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 12000);//-1代表一直等待，0表示不等待

        while (outputBufferIndex >= 0) {//大于等于0表示解码器有数据输出
            mediaCodec.releaseOutputBuffer(outputBufferIndex, true);

            outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);

            ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];

            final byte[] outData = new byte[bufferInfo.size];
            outputBuffer.get(outData);//将Buffer内的数据取出到字节数组中


        }
    }

    boolean a = true;
    int count = 0;

    public void shared(byte[] outData, int w, int h) {
        //渲染
        Log.e(TAG, "shared");
        byte[] yByte = new byte[h * w];
        byte[] uByte = new byte[h * w / 4];
        byte[] vByte = new byte[h * w / 4];

        System.arraycopy(outData, 0, yByte, 0, h * w);
        for (int i = h * w; i < outData.length; i += 2) {
            vByte[(i - h * w) / 2] = outData[i + 1];
            uByte[(i - h * w) / 2] = outData[i];
        }
        Log.e(TAG, "shared2");
        YUVData yuvData = new YUVData();
        yuvData.yuvW = h;
        yuvData.yuvH = w;
        yuvData.creatBuffer(yByte, uByte, vByte);
        myGlSurface1.uplaodTexture(yuvData);
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

    /**
     * Generates the presentation time for frame N, in microseconds.
     */
    private long computePresentationTime(long frameIndex) {
        return 132 + frameIndex * 1000000 / 20;
    }

    //解码h264数据
    private void offerDecoder(UiVideoData uiVideoData) {
        byte[] input = uiVideoData._data;
        try {
            ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();
            int inputBufferIndex = mediaCodec.dequeueInputBuffer(0);
            if (inputBufferIndex >= 0) {
                ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
                inputBuffer.clear();
                try {
                    inputBuffer.put(input, 0, input.length);
                } catch (Exception e) {
                    e.printStackTrace();
                }
                mediaCodec.queueInputBuffer(inputBufferIndex, 0, input.length, 0, 0);
            }
            MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();

            int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
            while (outputBufferIndex >= 0) {
                //If a valid surface was specified when configuring the codec,
                //passing true renders this output buffer to the surface.
                mediaCodec.releaseOutputBuffer(outputBufferIndex, true);
                outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
            }
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }
}
