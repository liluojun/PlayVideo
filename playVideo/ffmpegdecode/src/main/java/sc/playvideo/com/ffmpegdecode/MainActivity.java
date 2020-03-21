package sc.playvideo.com.ffmpegdecode;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;

import sc.playvideo.videonative.MediaNative;

public class MainActivity extends AppCompatActivity {

    private MediaNative mediaNative;
    private static final String TAG="MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mediaNative = new MediaNative();
        if(mediaNative.creatFfmpeg()==0){
            int i = mediaNative.initContext(1920, 1080, 20);
            Log.e(TAG,i+"");
        }
    }
}
