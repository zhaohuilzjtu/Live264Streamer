package com.hzy.live.streamer.instance;

import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.view.SurfaceHolder;

import java.util.List;

public enum CameraInstance implements Camera.PreviewCallback {

    INSTANCE;

    private Camera mCamera;
    private byte[] mPreviewBuffer;
    private Camera.PreviewCallback mPreviewCallback;
    private int mWidth;
    private int mHeight;

    public void setPreviewCallback(Camera.PreviewCallback cb) {
        mPreviewCallback = cb;
    }

    public void startPreview(int index, int width, int height, SurfaceHolder holder) {
        this.mWidth = width;
        this.mHeight = height;
        try {
            mCamera = Camera.open(index);
            mCamera.setPreviewDisplay(holder);
            Camera.Parameters parameters = this.mCamera.getParameters();
            parameters.setPreviewFormat(ImageFormat.NV21);
            parameters.setFlashMode(Camera.Parameters.FLASH_MODE_OFF);
            parameters.setWhiteBalance(Camera.Parameters.WHITE_BALANCE_AUTO);
            parameters.setSceneMode(Camera.Parameters.SCENE_MODE_AUTO);
            parameters.setPreviewSize(mWidth, mHeight);
            mCamera.setDisplayOrientation(90);
            List<String> focusModes = parameters.getSupportedFocusModes();
            if (focusModes.contains("continuous-video")) {
                parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
            }
            mCamera.setParameters(parameters);
            mPreviewBuffer = new byte[mWidth * mHeight * 3 / 2];
            mCamera.addCallbackBuffer(mPreviewBuffer);
            mCamera.setPreviewCallbackWithBuffer(this);
            mCamera.startPreview();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void stop() {
        this.mCamera.setPreviewCallback(null);
        this.mCamera.stopPreview();
        this.mCamera.release();
        this.mCamera = null;
    }

    @Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        if (mPreviewCallback != null) {
            mPreviewCallback.onPreviewFrame(data, camera);
        }
        mCamera.addCallbackBuffer(mPreviewBuffer);
    }
}
