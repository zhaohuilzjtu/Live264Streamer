//
// Created by huzongyao on 2019/4/22.
//

#ifndef LIVERTSPSTREAMER_H264ARRAYFRAMEDSOURCE_H
#define LIVERTSPSTREAMER_H264ARRAYFRAMEDSOURCE_H

#include "ByteArrayFramedSource.h"

const char H264marker[] = {0, 0, 0, 1};

class H264ArrayFramedSource : public ByteArrayFramedSource {
public:
    static H264ArrayFramedSource *
    createNew(UsageEnvironment &env, unsigned int queueSize, bool repeatConfig);

protected:
    H264ArrayFramedSource(UsageEnvironment &env, unsigned int queueSize, bool repeatConfig);

    virtual std::list<std::pair<unsigned char *, size_t> >
    splitFrames(unsigned char *frame, unsigned frameSize);

    unsigned char *extractFrame(unsigned char *frame, size_t &size, size_t &outsize);

private:
    std::string mSps;
    std::string mPps;
    bool mRepeatConfig;
};


#endif //LIVERTSPSTREAMER_H264ARRAYFRAMEDSOURCE_H
