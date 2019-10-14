package sc.playvideo.com.yuvencodedecode;

import android.content.Context;
import android.content.res.Configuration;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.util.Log;
import android.widget.Toast;

import sc.playvideo.com.yuvencodedecode.yuv.Encoder;
import sc.playvideo.com.yuvencodedecode.yuv.MyGlsurface;

import java.util.List;

/**
 * Created by HP on 2019/8/1.
 */

public class CameraSurfaceManage implements Camera.PreviewCallback {

    private static final String TAG = "CameraSurfaceManage";
    private Context context;
    private Camera camera;
    private Camera.Parameters parameters;
    private byte[] mPreviewBuffer;
    private MyGlsurface myGlsurface;
    private Encoder avcCodec;

    private int ScreenOrientation = Configuration.ORIENTATION_PORTRAIT;
    private int biterate;//码率
    public int framerate;//帧速率
    private int width;
    private int height;

    private boolean isPause;

    public CameraSurfaceManage(Context context, MyGlsurface myGlsurface) {
        this.context = context;
        this.myGlsurface = myGlsurface;
        framerate = 20;
        width = 352;
        height = 288;
        biterate = 128 * 1000;
//        initCamera(Camera.CameraInfo.CAMERA_FACING_FRONT);
    }

    /**
     * 初始化相机
     *
     * @param c
     */
    public void initCamera(int c) {
        //获取相机对象的实例
        try {
            if (camera == null) {
                camera = Camera.open(c);
            }
            camera.setPreviewCallbackWithBuffer(this);
            camera.setDisplayOrientation(90);
            if (parameters == null)
                parameters = camera.getParameters();

            //设置预览格式
            parameters.setPreviewFormat(ImageFormat.NV21);

            parameters.setPreviewFrameRate(framerate);

            List<Camera.Size> supportedPreviewSizes = parameters.getSupportedPreviewSizes();

            boolean flag = false;
            for (int i = 0; i < supportedPreviewSizes.size(); i++) {
                if (supportedPreviewSizes.get(i).width == width && supportedPreviewSizes.get(i).height == height) {
                    flag = true;
                    break;
                }
            }
            if (!flag) {
                Camera.Size size = supportedPreviewSizes.get(0);
                Camera.Size size2 = supportedPreviewSizes.get(supportedPreviewSizes.size() - 1);
                if (size.width - size2.width > 0) {
                    if (size2.width >= 352) {
                        width = size2.width;
                        height = size2.height;
                    } else {
                        for (int i = 0; i < supportedPreviewSizes.size(); i++) {
                            if (supportedPreviewSizes.get(i).width < width) {
                                width = supportedPreviewSizes.get(i).width;
                                height = supportedPreviewSizes.get(i).height;
                                break;
                            }
                        }

                    }
                } else {
                    if (size.width >= 352) {
                        width = size.width;
                        height = size.height;
                    } else {
                        for (int i = 0; i < supportedPreviewSizes.size(); i++) {
                            if (supportedPreviewSizes.get(i).width > width) {
                                width = supportedPreviewSizes.get(i).width;
                                height = supportedPreviewSizes.get(i).height;
                                if (width > 352)
                                    break;
                            }
                        }
                    }
                }
            }
            parameters.setPreviewSize(width, height);
            List<String> supportedFocusModes = parameters.getSupportedFocusModes();

            if (supportedFocusModes.size() > 0) {
                boolean mFocusModes = true;

                for (int i = 0; i < supportedFocusModes.size(); i++) {
                    if (Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE
                            .equals(supportedFocusModes.get(i))) {
                        parameters
                                .setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);// 连续对焦
                        camera.cancelAutoFocus();// 如果要实现连续的自动对焦，这一句必须加上
                        mFocusModes = false;
                        break;
                    }
                }

                if (mFocusModes) {
                    for (int j = 0; j < supportedFocusModes.size(); j++) {
                        if (Camera.Parameters.FOCUS_MODE_AUTO
                                .equals(supportedFocusModes.get(j))) {
                            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
                            break;
                        }
                    }
                }
            }
            List<int[]> supportedPreviewFpsRange = parameters.getSupportedPreviewFpsRange();
            //TODO 控制帧率20帧
            camera.setParameters(parameters);

            camera.setPreviewTexture(myGlsurface.initSurTexture());
            mPreviewBuffer = new byte[width * height * 3 / 2];
            camera.addCallbackBuffer(mPreviewBuffer);
            camera.setPreviewCallbackWithBuffer(this);
            //开始显示实时摄像机图像。
            camera.startPreview();

        } catch (Exception e) {
            Toast.makeText(context, "相机开启失败，请检查相机是否被占用或相机权限是否被开启", Toast.LENGTH_SHORT).show();
            e.printStackTrace();
        }
    }

    /**
     * 关闭本地摄像头
     */
    public void stopCamera() {
        try {
            if (camera != null) {
                camera.setPreviewCallbackWithBuffer(null);
                camera.stopPreview();
                camera.release();
                camera = null;
            }
            //停止相机数据编码
            if (avcCodec != null) {
                avcCodec.StopThread();
                avcCodec.release();
                avcCodec = null;
            }
            myGlsurface.clearSurfaceTexture();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public Camera getCamera() {
        if (camera != null)
            return camera;
        return null;
    }

    public void flush() {
        if (avcCodec != null)
            avcCodec.flush();
    }
    public void flush2() {
        if (avcCodec != null)
            avcCodec.flush2();
    }
    /**
     * 重置编码器
     *
     * @param b
     */
    public void resetMediaCode(boolean b) {
        if (camera != null) {
            try {
                isPause = true;
                if (b) {
                    ScreenOrientation = Configuration.ORIENTATION_PORTRAIT;
                } else {
                    ScreenOrientation = Configuration.ORIENTATION_LANDSCAPE;
                }
                if (avcCodec != null) {
                    avcCodec.StopThread();
                    avcCodec.release();
                    avcCodec = null;
                }
                myGlsurface.clearSurfaceTexture();
            } catch (Exception e) {
            } finally {
                isPause = false;
            }

        }
    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        Log.e(TAG, "onPreviewFrame");
        if (!isPause) {
            if (avcCodec == null) {
                int w = camera.getParameters().getPreviewSize().width;
                int h = camera.getParameters().getPreviewSize().height;
                if (ScreenOrientation == Configuration.ORIENTATION_PORTRAIT) {
                    avcCodec = new Encoder(context, w, h, framerate, biterate, myGlsurface);
                } else {
                    avcCodec = new Encoder(context, w, h, framerate, biterate, false, myGlsurface);
                }
            }
            avcCodec.encoder(data);

        }
        camera.addCallbackBuffer(mPreviewBuffer);
    }


}
