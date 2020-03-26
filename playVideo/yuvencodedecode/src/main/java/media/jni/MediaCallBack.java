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

    public MediaCallBack() {
    }

    public static void videoDataCallBackForJni(byte[] y, byte[] u, byte[] v, int w, int h) {
        mYUVData.yuvW = w;
        mYUVData.yuvH = h;
        mYUVData.creatBuffer(y, u, v);
        LiveDataBusController.getInstance().sendBusMessage(CameraActivity.TAG, Message.obtain(null, EventType.SHARE_VIDEO_DATA, mYUVData));
    }
}
