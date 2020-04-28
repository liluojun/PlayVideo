package sc.playvideo.com.yuvencodedecode;

import android.content.Context;
import android.content.Intent;
import android.hardware.Camera;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.os.Bundle;
import android.os.Environment;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import media.jni.JavaToNativeMethod;
import media.jni.MediaCallBack;
import sc.playvideo.com.yuvencodedecode.bean.UiVideoData;
import sc.playvideo.com.yuvencodedecode.mediaCode.Decoder;
import sc.playvideo.com.yuvencodedecode.yuv.MyGlsurface;
import sc.playvideo.com.yuvencodedecode.yuv.YUVData;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.util.Arrays;

public class CameraActivity extends AppCompatActivity implements LiveDataBusController.LiveDataBusCallBack {

    public static final String TAG = "CameraActivity";
    private CameraSurfaceManage cameraSurfaceManage;
    private MyGlsurface myGlSurface, myGlSurface1;

    private MediaCodec mediaCodec = null;
    private Button bt;
    private AudioTrack audioTrack;

    public static void start(Context context) {
        Intent intent = new Intent();
        intent.setClass(context, CameraActivity.class);
        context.startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera);
        LiveDataBusController.getInstance().addRegister(TAG, this, null);
        MediaCallBack.a(this);
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
                cameraSurfaceManage.startCutter720p();
            }
        });
        findViewById(R.id.bt3).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                JavaToNativeMethod.getInstence().openStream(/*"http://vjs.zencdn.net/v/oceans.mp4"*/"rtmp://58.200.131.2:1935/livetv/hunantv");
            }
        });
        cameraSurfaceManage = new CameraSurfaceManage(this, myGlSurface);
        cameraSurfaceManage.setWh(1280, 720);
        cameraSurfaceManage.initCamera(Camera.CameraInfo.CAMERA_FACING_FRONT);
        Decoder.getDecoder().setContext(this);
        audioTrack = audioInit();
    }

    @Override
    protected void onDestroy() {
        if (cameraSurfaceManage != null)
            cameraSurfaceManage.stopCamera();
        JavaToNativeMethod.getInstence().closeStream();
        super.onDestroy();
        LiveDataBusController.getInstance().removeRegister(TAG, this, null);
    }


    public void shared(byte[] outData, int w, int h) {
        //渲染
        Log.e(TAG, "shared=" + outData.length + "**w=" + w + "**h=" + h);
        byte[] yByte = new byte[h * w];
        byte[] uByte = new byte[h * w / 4];
        byte[] vByte = new byte[h * w / 4];
        System.arraycopy(outData, 0, yByte, 0, h * w);
        for (int i = h * w; i < outData.length; i += 2) {
            vByte[(i - h * w) / 2] = outData[i + 1];
            uByte[(i - h * w) / 2] = outData[i];
        }
        YUVData yuvData = new YUVData();
        yuvData.yuvW = w;
        yuvData.yuvH = h;
        yuvData.creatBuffer(yByte, uByte, vByte);
        myGlSurface1.uplaodTexture(yuvData);
    }

    public void shared(byte[] y, byte[] u, byte[] v, int w, int h) {
        //渲染

        YUVData yuvData = new YUVData();
        yuvData.yuvW = w;
        yuvData.yuvH = h;
        yuvData.creatBuffer(y, u, v);
        myGlSurface1.uplaodTexture(yuvData);
    }

    public void shared(YUVData yuv) {
        //渲染
        myGlSurface1.uplaodTexture(yuv);
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

    @Override
    public boolean handleMessage(Message msg) {
        switch (msg.what) {
            case EventType.SHARE_VIDEO_DATA: {
                shared((YUVData) msg.obj);
                break;
            }
            case EventType.SHARE_AUDIO_DATA:{
                audioTrack.write((byte[])msg.obj,0,((byte[])msg.obj).length);
                break;
            }
        }
        return false;
    }
    //初始化播放器
    public AudioTrack audioInit(){
        //音频码流 PCM 16位
        int audioFormat = AudioFormat.ENCODING_PCM_16BIT;
        int sampleRateInHz=44100;
        int buffSize = AudioTrack.getMinBufferSize(sampleRateInHz, AudioFormat.CHANNEL_OUT_STEREO, audioFormat);

        AudioTrack audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRateInHz, AudioFormat.CHANNEL_OUT_STEREO, audioFormat, buffSize, AudioTrack.MODE_STREAM);
        audioTrack.play();
        return audioTrack;

    }

}
