package com.hzy.live.streamer.widget;

import android.content.Context;
import android.graphics.PixelFormat;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class CameraSurfaceView extends SurfaceView
        implements SurfaceHolder.Callback {

    public static final String TAG = "CameraSurfaceView";
    SurfaceHolder mSurfaceHolder;
    Context mContext;

    public CameraSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        this.mSurfaceHolder = getHolder();
        this.mContext = getContext();
        this.mSurfaceHolder.setFormat(PixelFormat.TRANSPARENT);
        this.mSurfaceHolder.addCallback(this);
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }
}
