//
// Created by huzongyao on 2018/11/20.
//

#ifndef LIVE555RTSPCAMERA_LIVE555API_H
#define LIVE555RTSPCAMERA_LIVE555API_H

#ifdef __cplusplus
extern "C" {
#endif

#include <jni.h>

#ifdef NATIVE_LOG
#define LOG_TAG "NATIVE.LOG"
#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#else
#define LOGD(...) do{}while(0)
#define LOGI(...) do{}while(0)
#define LOGW(...) do{}while(0)
#define LOGE(...) do{}while(0)
#define LOGF(...) do{}while(0)
#endif

#define JAVA_FUNC(f) Java_com_hzy_rtsp_live555_Live555Api_##f

JNIEXPORT jstring JNICALL
JAVA_FUNC(getVersionInfo)(JNIEnv *env, jclass type);

JNIEXPORT jlong JNICALL
JAVA_FUNC(createNew)(JNIEnv *env, jclass type);

JNIEXPORT jint JNICALL
JAVA_FUNC(addUserRecord)(JNIEnv *env, jclass type, jlong instanceId, jstring userName_,
                         jstring password_);

JNIEXPORT jint JNICALL
JAVA_FUNC(startServer)(JNIEnv *env, jclass type, jlong instanceId, jint port, jstring streamName_);

JNIEXPORT jstring JNICALL
JAVA_FUNC(getStreamUrl)(JNIEnv *env, jclass type, jlong instanceId);

JNIEXPORT jint JNICALL
JAVA_FUNC(doEventLoop)(JNIEnv *env, jclass type, jlong instanceId);

JNIEXPORT jint JNICALL
JAVA_FUNC(feedH264Data)(JNIEnv *env, jclass type, jlong instanceId, jbyteArray data_);

JNIEXPORT jint JNICALL
JAVA_FUNC(stopServer)(JNIEnv *env, jclass type, jlong instanceId);

#ifdef __cplusplus
}
#endif

#endif //LIVE555RTSPCAMERA_LIVE555API_H