//
// Created by huzongyao on 2019/4/20.
//

#ifndef LIVERTSPSTREAMER_BYTEARRAYFRAMEDSOURCE_H
#define LIVERTSPSTREAMER_BYTEARRAYFRAMEDSOURCE_H


#include <include/FramedSource.hh>
#include <string>
#include <list>

class ByteArrayFramedSource : public FramedSource {
public:
    struct Frame {
        Frame(char *buffer, int size, timeval timestamp)
                : mBuffer(buffer), mSize(size), mTimestamp(timestamp) {};

        ~Frame() { delete mBuffer; };
        char *mBuffer;
        int mSize;
        timeval mTimestamp;
    };

    struct RawData {
        ~RawData() { delete mBuffer; };
        char *mBuffer;
        int mSize;
    };

    static ByteArrayFramedSource *createNew(UsageEnvironment &env, unsigned int queueSize);

    std::string getAuxLine() { return mAuxLine; };

    void pushRawData(char *data, unsigned int dataSize);

protected:
    ByteArrayFramedSource(UsageEnvironment &env, unsigned int queueSize);

    virtual ~ByteArrayFramedSource();

    void *getFrameThread();

    void deliverFrame();

    int getNextFrame();

    void processFrame(char *frame, int frameSize, const timeval &ref);

    void queueFrame(char *frame, int frameSize, const timeval &tv);

    virtual std::list<std::pair<unsigned char *, size_t> >
    splitFrames(unsigned char *frame, unsigned frameSize);

    virtual void doGetNextFrame();

    virtual void doStopGettingFrames();

    static void *
    threadStub(void *clientData) { return ((ByteArrayFramedSource *) clientData)->getFrameThread(); };

    static void
    deliverFrameStub(void *clientData) { ((ByteArrayFramedSource *) clientData)->deliverFrame(); };

    std::list<Frame *> mCaptureQueue;
    std::list<RawData *> mRawDataQueue;
    EventTriggerId mEventTriggerId;
    unsigned int mQueueSize;
    pthread_t mThreadId;
    pthread_mutex_t mMutex;
    pthread_mutex_t mMutexRaw;
    std::string mAuxLine;
    char volatile mNeedReadFrame;
};


#endif //LIVERTSPSTREAMER_BYTEARRAYFRAMEDSOURCE_H
