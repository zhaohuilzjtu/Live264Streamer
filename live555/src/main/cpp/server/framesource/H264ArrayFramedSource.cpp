//
// Created by huzongyao on 2019/4/22.
//

#include <include/Base64.hh>
#include <sstream>
#include <iomanip>
#include "H264ArrayFramedSource.h"
#include "../../live555api.h"

H264ArrayFramedSource::H264ArrayFramedSource(UsageEnvironment &env, unsigned int queueSize,
                                             bool repeatConfig)
        : ByteArrayFramedSource(env, queueSize), mRepeatConfig(repeatConfig) {}

H264ArrayFramedSource *
H264ArrayFramedSource::createNew(UsageEnvironment &env, unsigned int queueSize, bool repeatConfig) {
    return new H264ArrayFramedSource(env, queueSize, repeatConfig);
}

unsigned char *
H264ArrayFramedSource::extractFrame(unsigned char *frame, size_t &size, size_t &outsize) {
    unsigned char *outFrame = NULL;
    outsize = 0;
    if ((size >= sizeof(H264marker)) && (memcmp(frame, H264marker, sizeof(H264marker)) == 0)) {
        size -= sizeof(H264marker);
        outFrame = &frame[sizeof(H264marker)];
        outsize = size;
        for (int i = 0; i + sizeof(H264marker) < size; ++i) {
            if (memcmp(&outFrame[i], H264marker, sizeof(H264marker)) == 0) {
                outsize = (size_t) i;
                break;
            }
        }
        size -= outsize;
    }
    return outFrame;
}

std::list<std::pair<unsigned char *, size_t> >
H264ArrayFramedSource::splitFrames(unsigned char *frame, unsigned frameSize) {
    std::list<std::pair<unsigned char *, size_t> > frameList;

    size_t bufSize = frameSize;
    size_t size = 0;
    unsigned char *buffer = this->extractFrame(frame, bufSize, size);
    while (buffer != NULL) {
        switch (buffer[0] & 0x1F) {
            case 7:
                LOGI("SPS size:%d", size);
                mSps.assign((char *) buffer, size);
                break;
            case 8:
                LOGI("PPS size:%d", size);
                mPps.assign((char *) buffer, size);
                break;
            case 5:
                LOGI("IDR size:%d", size);
                if (mRepeatConfig && !mSps.empty() && !mPps.empty()) {
                    frameList.push_back(
                            std::make_pair((unsigned char *) mSps.c_str(), mSps.size()));
                    frameList.push_back(
                            std::make_pair((unsigned char *) mPps.c_str(), mPps.size()));
                }
                break;
            default:
                break;
        }

        if (mAuxLine.empty() && !mSps.empty() && !mPps.empty()) {
            u_int32_t profile_level_id = 0;
            if (mSps.size() >= 4)
                profile_level_id = (u_int32_t) ((mSps[1] << 16) | (mSps[2] << 8) | mSps[3]);
            char *sps_base64 = base64Encode(mSps.c_str(), mSps.size());
            char *pps_base64 = base64Encode(mPps.c_str(), mPps.size());

            std::ostringstream os;
            os << "profile-level-id=" << std::hex << std::setw(6) << profile_level_id;
            os << ";sprop-parameter-sets=" << sps_base64 << "," << pps_base64;
            mAuxLine.assign(os.str());

            free(sps_base64);
            free(pps_base64);
            LOGI("%s", mAuxLine.c_str());
        }
        frameList.push_back(std::make_pair(buffer, size));
        buffer = this->extractFrame(&buffer[size], bufSize, size);
    }
    return frameList;
}

