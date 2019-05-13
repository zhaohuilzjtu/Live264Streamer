package com.hzy.live.streamer.encoder;

import android.annotation.TargetApi;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.os.Build;

import java.nio.ByteBuffer;

public class H264BufferEncoder extends BaseMediaEncoder {

    private static final int FRAME_RATE = 15;
    private static final int COMPRESS_RATIO = 64;
    private static final int I_FRAME_INTERVAL = 5;

    private int mWidth;
    private int mHeight;

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public H264BufferEncoder(int width, int height) {
        mWidth = width;
        mHeight = height;

        MediaFormat mediaFormat =
                MediaFormat.createVideoFormat(MediaFormat.MIMETYPE_VIDEO_AVC, width, height);
        int bitRate = height * width * 24 * FRAME_RATE / COMPRESS_RATIO;
        mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, bitRate);
        mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, FRAME_RATE);
        int colorFormat = MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar;
        mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, colorFormat);
        mediaFormat.setInteger(MediaFormat.KEY_BITRATE_MODE,
                MediaCodecInfo.EncoderCapabilities.BITRATE_MODE_CQ);
        mediaFormat.setInteger(MediaFormat.KEY_COMPLEXITY,
                MediaCodecInfo.EncoderCapabilities.BITRATE_MODE_CQ);
        mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, I_FRAME_INTERVAL);
        try {
            mMediaCodec = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_VIDEO_AVC);
            mMediaCodec.configure(mediaFormat, null, null,
                    MediaCodec.CONFIGURE_FLAG_ENCODE);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public void queenFrame(byte[] input) {
        swapNV21toI420SemiPlanar(input, mWidth, mHeight);
        int inputBufferIndex = mMediaCodec.dequeueInputBuffer(mTimeout);
        if (inputBufferIndex >= 0) {
            ByteBuffer inputBuffer = mMediaCodec.getInputBuffer(inputBufferIndex);
            if (inputBuffer != null) {
                inputBuffer.clear();
                inputBuffer.put(input);
                mMediaCodec.queueInputBuffer(inputBufferIndex,
                        0, input.length, System.currentTimeMillis(), 0);
            }
        }
    }

    private void swapNV21toI420SemiPlanar(byte[] source, int width, int height) {
        for (int i = width * height; i < source.length; i += 2) {
            byte temp = source[i];
            source[i] = source[i + 1];
            source[i + 1] = temp;
        }
    }
}
