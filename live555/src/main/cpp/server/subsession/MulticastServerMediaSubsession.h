//
// Created by huzongyao on 2019/4/30.
//

#ifndef LIVERTSPSTREAMER_MULTICASTSERVERMEDIASUBSESSION_H
#define LIVERTSPSTREAMER_MULTICASTSERVERMEDIASUBSESSION_H


#include <include/PassiveServerMediaSubsession.hh>
#include "H264StreamBaseSubsession.h"

class MulticastServerMediaSubsession :
        public PassiveServerMediaSubsession, public H264StreamBaseSubsession {
public:
    static MulticastServerMediaSubsession *
    createNew(UsageEnvironment &env, struct in_addr destinationAddress, Port rtpPortNum,
              Port rtcpPortNum, int ttl, StreamReplicator *replicator
    );

protected:
    MulticastServerMediaSubsession(StreamReplicator *replicator, RTPSink *rtpSink,
                                   RTCPInstance *rtcpInstance)
            : PassiveServerMediaSubsession(*rtpSink, rtcpInstance),
              H264StreamBaseSubsession(replicator), mRtpSink(rtpSink) {};

    virtual char const *sdpLines();

    virtual char const *getAuxSDPLine(RTPSink *rtpSink, FramedSource *inputSource);

protected:
    RTPSink *mRtpSink;
    std::string mSDPLines;
};


#endif //LIVERTSPSTREAMER_MULTICASTSERVERMEDIASUBSESSION_H
