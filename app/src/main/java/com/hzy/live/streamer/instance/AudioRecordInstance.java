package com.hzy.live.streamer.instance;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

import com.hzy.live.streamer.encoder.AACBufferEncoder;
import com.hzy.live.streamer.encoder.EncoderListener;

public enum AudioRecordInstance implements EncoderListener, Runnable {

    INSTANCE;

    private int mAudioSource = MediaRecorder.AudioSource.MIC;
    private int mSampleRateInHz = 44100;
    private int mChannelConfig = AudioFormat.CHANNEL_IN_STEREO;
    private int mAudioFormat = AudioFormat.ENCODING_PCM_16BIT;
    private int mBufferSizeInBytes;

    private AACBufferEncoder mAudioEncoder;
    private AudioRecord mAudioRecord;
    private byte[] mAudioBuffer;
    private volatile boolean mIsRunning;

    AudioRecordInstance() {
        mBufferSizeInBytes = AudioRecord.getMinBufferSize(mSampleRateInHz,
                mChannelConfig, mAudioFormat) * 2;
    }

    public void startRecord() {
        if (!mIsRunning) {
            mAudioEncoder = new AACBufferEncoder(mSampleRateInHz, mBufferSizeInBytes);
            mAudioEncoder.setListener(this);
            mAudioRecord = new AudioRecord(mAudioSource, mSampleRateInHz,
                    mChannelConfig, mAudioFormat, mBufferSizeInBytes);
            mAudioBuffer = new byte[mBufferSizeInBytes];
            if (mAudioRecord.getState() == AudioRecord.STATE_INITIALIZED) {
                mAudioEncoder.start();
                mAudioRecord.startRecording();
                mIsRunning = true;
                new Thread(this).start();
            }
        }
    }

    public void stopRecord() {
        if (mIsRunning) {
            mIsRunning = false;
            mAudioRecord.stop();
            mAudioRecord.release();
            mAudioEncoder.stop();
        }
    }

    @Override
    public void run() {
        while (mIsRunning) {
            int readSize = mAudioRecord.read(mAudioBuffer, 0, mBufferSizeInBytes);
            if (AudioRecord.ERROR_INVALID_OPERATION != readSize) {
                mAudioEncoder.queenBuffer(mAudioBuffer, readSize);
            }
        }
    }

    @Override
    public void onEncodeRaw(byte[] output) {

    }
}
