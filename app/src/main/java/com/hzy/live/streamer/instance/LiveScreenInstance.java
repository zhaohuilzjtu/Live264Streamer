package com.hzy.live.streamer.instance;

import android.content.Context;
import android.content.Intent;

import com.hzy.live.streamer.encoder.EncoderListener;
import com.hzy.live.streamer.encoder.H264VDisplayEncoder;

public enum LiveScreenInstance implements EncoderListener {
    INSTANCE;

    private H264VDisplayEncoder mDisplayEncoder;
    private boolean mIsRunning = false;
    private Live555Instance mInstance;

    LiveScreenInstance() {
        mInstance = Live555Instance.INSTANCE;
    }

    public void startScreenStream(Context context, Intent intent) {
        if (!mIsRunning) {
            startStreamer();
            mDisplayEncoder = new H264VDisplayEncoder(context,
                    360, 640,
                    intent.getIntExtra("resultCode", 0),
                    intent.getParcelableExtra("data"));
            mDisplayEncoder.setListener(this);
            mDisplayEncoder.start();
            mIsRunning = true;
        }
    }

    public void stopScreenStream() {
        if (mIsRunning) {
            mIsRunning = false;
            mDisplayEncoder.stop();
            mInstance.stopServer();
        }
    }

    private void startStreamer() {
        mInstance.start();
    }

    @Override
    public void onEncodeRaw(byte[] output) {
        try {
            mInstance.feedH264Data(output);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
