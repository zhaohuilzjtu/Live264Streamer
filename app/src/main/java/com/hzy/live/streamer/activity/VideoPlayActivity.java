package com.hzy.live.streamer.activity;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.widget.VideoView;

import com.hzy.live.streamer.R;

import butterknife.BindView;
import butterknife.ButterKnife;

public class VideoPlayActivity extends AppCompatActivity {

    @BindView(R.id.video_view)
    VideoView mVideoView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video_play);
        ButterKnife.bind(this);
        try {
            mVideoView.setVideoURI(getIntent().getData());
            mVideoView.start();
        } catch (Exception e) {
            e.printStackTrace();
            finish();
        }
    }
}
