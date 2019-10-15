package sc.playvideo.com.yuvencodedecode;

import android.app.Application;

public class MyApplication extends Application {
    static {
        System.loadLibrary("media");
        System.loadLibrary("yuv");
    }
}
