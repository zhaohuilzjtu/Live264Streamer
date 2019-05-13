# Live264Streamer
A Project To Study Stream H264 Video With Live555

[![Travis](https://img.shields.io/appveyor/ci/gruntjs/grunt.svg)](https://github.com/huzongyao/Live264Streamer)
[![Travis](https://img.shields.io/badge/API-21+-brightgreen.svg)](https://github.com/huzongyao/Live264Streamer)
[![Travis](https://img.shields.io/badge/live555-v1.0.0-brightgreen.svg)](https://github.com/huzongyao/Live264Streamer/releases)

### Introduction
学习多媒体相关知识，相机/音视频编解码/网络直播RTSP等

#### 实现功能
* 使用RTSP共享设备屏幕录屏
* 使用RTSP共享相机视频直播
* 扫码播放RTSP直播

#### 下载体验
* apk下载： https://github.com/huzongyao/Live264Streamer/releases
* 播放器VLC：https://www.videolan.org/

#### Screenshot
| Stream Push | VLC Play  | VLC Play |
| ----------- |:-----------:| ---------:|
| ![screenshot](https://github.com/huzongyao/Live264Streamer/blob/master/misc/screen0.png?raw=true)| ![screenshot](https://github.com/huzongyao/Live264Streamer/blob/master/misc/screen1.png?raw=true)| ![screenshot](https://github.com/huzongyao/Live264Streamer/blob/master/misc/screen2.png?raw=true)|

#### 涉及知识点
* 屏幕录像并将其硬编成H264:
1. 操作屏幕录像需要Android5.0以上支持，录制之前会弹出动态权限申请弹框。
```java
mProjectionManager = (MediaProjectionManager)getSystemService(Context.MEDIA_PROJECTION_SERVICE);
startActivityForResult(mProjectionManager.createScreenCaptureIntent(), REQUEST_CODE);
```
2. 在onActivityResult返回的resultCode和resultData可用于获取MediaProjection，使用MediaProjection和硬编码器创建的surface，
就可以创建一个VirtualDisplay，且VirtualDisplay显示内容会被源源不断送到编码器编码，直到MediaProjection.stop()：
```java
mMediaCodec = MediaCodec.createEncoderByType(MIME_TYPE);
Surface surface = mMediaCodec.createInputSurface();
mMediaProjection = projectionManager.getMediaProjection(resultCode, resultData);
mVirtualDisplay = mMediaProjection.createVirtualDisplay("H264VDisplay",
                    width, height, dpi,
                    DisplayManager.VIRTUAL_DISPLAY_FLAG_PRESENTATION,
                    surface, null, null);
```


* 摄像头拍摄并硬编码H264:
1. YUV格式帧数据：

| I420(YUV420P) | YV12（YUV420P）| NV12(YUV420SP) | NV21(YUV420SP) |
| :- | :- | :- | :- |
| YYYYYYYYUUVV |YYYYYYYYVVUU | YYYYYYYYUVUV | YYYYYYYYVUVU |

摄像头采集的NV21帧数据转换成H264硬编码器输入使用的YUV420SemiPlanar。
对于转换操作，可以引入Libyuv库来操作，也可以用java方式。实际操作就是UV互换，
把VUVU转换为UVUV。(假如想让输出的图像变成黑白，只需要把U和V都写死成-128即可)
``` java
Camera.Parameters parameters = this.mCamera.getParameters();
parameters.setPreviewFormat(ImageFormat.NV21);  //摄像头配置
...
int colorFormat = MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar;
mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, colorFormat); //编码器配置
...
// 转换操作
private void swapNV21toI420SemiPlanar(byte[] source, int width, int height) {
    for (int i = width * height; i < source.length; i += 2) {
        byte temp = source[i];
        source[i] = source[i + 1];
        source[i + 1] = temp;
    }
}
```

* 硬编码器使用
1. Android4.1之后Android系统才统一了硬件编解码接口。
硬件编码的过程类似于我们过机场安检，安检机配套了若干装物品的塑料盒子，我们把需要处理的物品放进盒子。
安检过程不需要我们处理，只需要到出口轮询并从的盒子里拿走已处理完的物品即可。
Android系统硬件编码器也提供了若干这样的盒子，便是输入缓冲队列和输入缓冲队列，我们把要编码的数据放到
可用的输入缓冲去，再到输出缓冲区取处理好的数据即可。
``` java
MediaCodec codec = MediaCodec.createByCodecName(name);
codec.start();
for (;;) {
    // 写入输入缓冲区
    int inputBufferId = codec.dequeueInputBuffer(timeoutUs);
    if (inputBufferId >= 0) {
        ByteBuffer inputBuffer = codec.getInputBuffer(…);
        // fill inputBuffer with valid data
        …
        codec.queueInputBuffer(inputBufferId, …);
    }
    // 读取输出缓冲区
    int outputBufferId = codec.dequeueOutputBuffer(…);
    if (outputBufferId >= 0) {
        ByteBuffer outputBuffer = codec.getOutputBuffer(outputBufferId);
        MediaFormat bufferFormat = codec.getOutputFormat(outputBufferId); // option A
        // bufferFormat is identical to outputFormat
        // outputBuffer is ready to be processed or rendered.
        …
        codec.releaseOutputBuffer(outputBufferId, …);
    }
}
```

2. Android5.0以后系统提供了异步回调模式， 
``` java
MediaCodec codec = MediaCodec.createByCodecName(name);
codec.setCallback(new MediaCodec.Callback() {
    @Override
    void onInputBufferAvailable(MediaCodec mc, int inputBufferId) {
    ByteBuffer inputBuffer = codec.getInputBuffer(inputBufferId);
    // fill inputBuffer with valid data
    …
    codec.queueInputBuffer(inputBufferId, …);
    }

    @Override
    void onOutputBufferAvailable(MediaCodec mc, int outputBufferId, …) {
    ByteBuffer outputBuffer = codec.getOutputBuffer(outputBufferId);
    MediaFormat bufferFormat = codec.getOutputFormat(outputBufferId); // option A
    // bufferFormat is equivalent to mOutputFormat
    // outputBuffer is ready to be processed or rendered.
    …
    codec.releaseOutputBuffer(outputBufferId, …);
    }
});
```

### Dependencies
* [Live555](http://www.live555.com/) 
* Live555最新源码下载：http://www.live555.com/liveMedia/public/

### Reference
* [LiveStreamer](https://github.com/papan01/LiveStreamer)
* [v4l2rtspserver](https://github.com/mpromonet/v4l2rtspserver) 
* [larker](https://github.com/leepood/larker)

### About Me
 * GitHub: [https://huzongyao.github.io/](https://huzongyao.github.io/)
 * ITEye博客：[http://hzy3774.iteye.com/](http://hzy3774.iteye.com/)
 * 新浪微博: [http://weibo.com/hzy3774](http://weibo.com/hzy3774)

### Contact To Me
 * QQ: [377406997](http://wpa.qq.com/msgrd?v=3&uin=377406997&site=qq&menu=yes)
 * Gmail: [hzy3774@gmail.com](mailto:hzy3774@gmail.com)
 * Foxmail: [hzy3774@qq.com](mailto:hzy3774@qq.com)
 * WeChat: hzy3774

  ![image](https://raw.githubusercontent.com/hzy3774/AndroidP7zip/master/misc/wechat.png)
