package sc.playvideo.com.yuvencodedecode;

import android.app.Application;

public class MyApplication extends Application {
    static {
        System.loadLibrary("media");
        System.loadLibrary("avcodec");
        System.loadLibrary("avdevice");
        System.loadLibrary("avfilter");
        System.loadLibrary("avformat");
        System.loadLibrary("avutil");
        System.loadLibrary("mediaffmpeg");
        System.loadLibrary("postproc");
        System.loadLibrary("swresample");
        System.loadLibrary("swscale");
        System.loadLibrary("yuv");
    }
}
