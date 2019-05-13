//
// Created by huzongyao on 2019/4/30.
//

#ifndef LIVERTSPSTREAMER_H264BASESUBSESSION_H
#define LIVERTSPSTREAMER_H264BASESUBSESSION_H


#include <include/StreamReplicator.hh>
#include <include/RTPSink.hh>
#include "../framesource/ByteArrayFramedSource.h"

class H264StreamBaseSubsession {
public:
    H264StreamBaseSubsession(StreamReplicator *replicator) : mReplicator(replicator) {};

    static FramedSource *createSource(UsageEnvironment &env, FramedSource *videoES);

    static RTPSink *createSink(UsageEnvironment &env, Groupsock *rtpGroupsock,
                               unsigned char rtpPayloadTypeIfDynamic);

    char const *getAuxLine(ByteArrayFramedSource *source, unsigned char rtpPayloadType);

protected:
    StreamReplicator *mReplicator;
};


#endif //LIVERTSPSTREAMER_H264BASESUBSESSION_H
