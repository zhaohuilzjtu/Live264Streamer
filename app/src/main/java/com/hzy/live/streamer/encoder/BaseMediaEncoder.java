package com.hzy.live.streamer.encoder;

import android.annotation.TargetApi;
import android.media.MediaCodec;
import android.os.Build;

import java.nio.ByteBuffer;

public abstract class BaseMediaEncoder implements Runnable {

    protected MediaCodec mMediaCodec;
    protected MediaCodec.BufferInfo mBufferInfo;
    protected Thread mListenThread;

    protected EncoderListener mListener;
    protected long mTimeout = 10_000;
    protected volatile boolean mIsRunning;

    public BaseMediaEncoder() {
        mListenThread = new Thread(this);
        mBufferInfo = new MediaCodec.BufferInfo();
    }

    public void setTimeout(long timeout) {
        mTimeout = timeout;
    }

    public void setListener(EncoderListener listener) {
        this.mListener = listener;
    }

    public boolean isIsRunning() {
        return mIsRunning;
    }

    @Override
    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public void run() {
        boolean streamEnded = false;
        while (!streamEnded && mIsRunning) {
            try {
                int outputBufferIndex = mMediaCodec.dequeueOutputBuffer(mBufferInfo, mTimeout);
                if (outputBufferIndex >= 0) {
                    ByteBuffer outputBuffer = mMediaCodec.getOutputBuffer(outputBufferIndex);
                    if (outputBuffer != null) {
                        outputBuffer.position(mBufferInfo.offset);
                        outputBuffer.limit(mBufferInfo.offset + mBufferInfo.size);
                        byte[] outBuffer = new byte[mBufferInfo.size];
                        outputBuffer.get(outBuffer, 0, mBufferInfo.size);
                        if (mListener != null) {
                            mListener.onEncodeRaw(outBuffer);
                        }
                        streamEnded =
                                (mBufferInfo.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0;
                        mMediaCodec.releaseOutputBuffer(outputBufferIndex, false);
                    }
                }
            } catch (Throwable e) {
                e.printStackTrace();
            }
        }
    }

    public void stop() {
        try {
            mIsRunning = false;
            mMediaCodec.stop();
            mMediaCodec.release();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public void start() {
        try {
            mMediaCodec.start();
            mListenThread.start();
            mIsRunning = true;
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
