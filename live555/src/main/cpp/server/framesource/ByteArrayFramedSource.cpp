//
// Created by huzongyao on 2019/4/20.
//

#include <pthread.h>
#include "ByteArrayFramedSource.h"
#include "../../live555api.h"

ByteArrayFramedSource *
ByteArrayFramedSource::createNew(UsageEnvironment &env, unsigned int queueSize) {
    return new ByteArrayFramedSource(env, queueSize);
}

ByteArrayFramedSource::ByteArrayFramedSource(UsageEnvironment &env, unsigned int queueSize)
        : FramedSource(env), mQueueSize(queueSize), mNeedReadFrame(1) {

    mEventTriggerId = envir().taskScheduler()
            .createEventTrigger(ByteArrayFramedSource::deliverFrameStub);
    memset(&mThreadId, 0, sizeof(mThreadId));
    memset(&mMutex, 0, sizeof(mMutex));
    memset(&mMutexRaw, 0, sizeof(mMutexRaw));

    // start thread
    pthread_mutex_init(&mMutex, NULL);
    pthread_mutex_init(&mMutexRaw, NULL);
    pthread_create(&mThreadId, NULL, threadStub, this);
}

void *ByteArrayFramedSource::getFrameThread() {
    while (mNeedReadFrame) {
        this->getNextFrame();
    }
    return NULL;
}

void ByteArrayFramedSource::deliverFrame() {
    if (isCurrentlyAwaitingData()) {
        fDurationInMicroseconds = 0;
        fFrameSize = 0;

        pthread_mutex_lock(&mMutex);
        if (!mCaptureQueue.empty()) {
            timeval curTime;
            gettimeofday(&curTime, NULL);
            Frame *frame = mCaptureQueue.front();
            mCaptureQueue.pop_front();
            if (frame->mSize > fMaxSize) {
                fFrameSize = fMaxSize;
                fNumTruncatedBytes = frame->mSize - fMaxSize;
            } else {
                fFrameSize = (unsigned int) frame->mSize;
            }
            timeval diff;
            timersub(&curTime, &(frame->mTimestamp), &diff);
            fPresentationTime = frame->mTimestamp;
            memcpy(fTo, frame->mBuffer, fFrameSize);
            delete frame;
        }
        pthread_mutex_unlock(&mMutex);

        if (fFrameSize > 0) {
            // send Frame to the consumer
            FramedSource::afterGetting(this);
        }
    }
}

void ByteArrayFramedSource::pushRawData(char *d, unsigned int dataSize) {
    pthread_mutex_lock(&mMutexRaw);

    RawData *data = (RawData *) malloc(sizeof(RawData));
    memset(data, 0, sizeof(RawData));
    data->mBuffer = d;
    data->mSize = dataSize;
    mRawDataQueue.push_back(data);

    pthread_mutex_unlock(&mMutexRaw);
}

int ByteArrayFramedSource::getNextFrame() {
    timeval ref;
    gettimeofday(&ref, NULL);
    int frameSize = 0;
    pthread_mutex_lock(&mMutexRaw);

    // fetch and process data in another thread
    if (!mRawDataQueue.empty()) {
        RawData *rawData = mRawDataQueue.front();
        mRawDataQueue.pop_front();
        timeval tv;
        gettimeofday(&tv, NULL);
        timeval diff;
        timersub(&tv, &ref, &diff);
        frameSize = rawData->mSize;
        processFrame(rawData->mBuffer, frameSize, ref);
    }

    pthread_mutex_unlock(&mMutexRaw);
    return frameSize;
}

void ByteArrayFramedSource::processFrame(char *frame, int frameSize, const timeval &ref) {
    timeval tv;
    gettimeofday(&tv, NULL);
    timeval diff;
    timersub(&tv, &ref, &diff);

    std::list<std::pair<unsigned char *, size_t> > frameList = this->splitFrames(
            (unsigned char *) frame, (unsigned int) frameSize);
    while (!frameList.empty()) {
        std::pair<unsigned char *, size_t> &ff = frameList.front();
        size_t size = ff.second;
        char *buf = new char[size];
        memcpy(buf, ff.first, size);
        queueFrame(buf, size, ref);
        frameList.pop_front();
    }
}

void ByteArrayFramedSource::queueFrame(char *frame, int frameSize, const timeval &tv) {
    pthread_mutex_lock(&mMutex);
    while (mCaptureQueue.size() >= mQueueSize) {
        delete mCaptureQueue.front();
        mCaptureQueue.pop_front();
    }
    mCaptureQueue.push_back(new Frame(frame, frameSize, tv));
    pthread_mutex_unlock(&mMutex);
    // post an event to ask to deliver the frame
    envir().taskScheduler().triggerEvent(mEventTriggerId, this);
}

void ByteArrayFramedSource::doStopGettingFrames() {
    LOGI("ByteArrayFramedSource::doStopGettingFrames()");
    FramedSource::doStopGettingFrames();
}

ByteArrayFramedSource::~ByteArrayFramedSource() {
    mNeedReadFrame = 0;
    envir().taskScheduler().deleteEventTrigger(mEventTriggerId);
    pthread_join(mThreadId, NULL);
    pthread_mutex_destroy(&mMutex);
    pthread_mutex_destroy(&mMutexRaw);
}

void ByteArrayFramedSource::doGetNextFrame() {
    deliverFrame();
}

std::list<std::pair<unsigned char *, size_t> >
ByteArrayFramedSource::splitFrames(unsigned char *frame, unsigned frameSize) {
    std::list<std::pair<unsigned char *, size_t> > frameList;
    if (frame != NULL) {
        frameList.push_back(std::make_pair(frame, frameSize));
    } else {
        LOGI("DisplayDeviceSource::splitFrames  frame empty");
    }
    return frameList;
}
