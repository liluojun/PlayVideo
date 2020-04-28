package media.jni;

import android.os.Message;
import android.util.Log;

import sc.playvideo.com.yuvencodedecode.CameraActivity;
import sc.playvideo.com.yuvencodedecode.EventType;
import sc.playvideo.com.yuvencodedecode.LiveDataBusController;
import sc.playvideo.com.yuvencodedecode.yuv.YUVData;

public class MediaCallBack {
    public static final String TAG = "MediaCallBack";
    private static YUVData mYUVData = new YUVData();
    public static CameraActivity CameraActivity;

    public static void a(CameraActivity m) {
        CameraActivity = m;
    }

    public MediaCallBack() {
    }

    public static void videoDataCallBackForJni(byte[] y, byte[] u, byte[] v, int w, int h) {
        YUVData mYUVData = new YUVData();
        mYUVData.yuvW = w;
        mYUVData.yuvH = h;
        mYUVData.creatBuffer(y, u, v);
        if (CameraActivity != null) {
            CameraActivity.handleMessage(Message.obtain(null, EventType.SHARE_VIDEO_DATA, mYUVData));
        }
    }

    public static void audioDataCallBackForJni(byte[] audio) {
        Log.e(TAG, "AUDIO");
        if (CameraActivity != null) {
            CameraActivity.handleMessage(Message.obtain(null, EventType.SHARE_AUDIO_DATA, audio));
        }
    }


}
