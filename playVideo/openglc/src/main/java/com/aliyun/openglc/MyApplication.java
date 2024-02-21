package com.aliyun.openglc;

import android.app.Application;

public class MyApplication extends Application {
    static {
        System.loadLibrary("renderLib");
    }
}
