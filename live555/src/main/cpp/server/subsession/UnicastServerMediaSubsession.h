//
// Created by huzongyao on 2019/4/30.
//

#ifndef LIVERTSPSTREAMER_UNICASTSERVERMEDIASUBSESSION_H
#define LIVERTSPSTREAMER_UNICASTSERVERMEDIASUBSESSION_H


#include <include/OnDemandServerMediaSubsession.hh>
#include "H264StreamBaseSubsession.h"

class UnicastServerMediaSubsession
        : public OnDemandServerMediaSubsession, public H264StreamBaseSubsession {
public:
    static UnicastServerMediaSubsession *
    createNew(UsageEnvironment &env, StreamReplicator *replicator);

protected:
    UnicastServerMediaSubsession(UsageEnvironment &env, StreamReplicator *replicator)
            : OnDemandServerMediaSubsession(env, False), H264StreamBaseSubsession(replicator) {};

    virtual FramedSource *createNewStreamSource(unsigned clientSessionId, unsigned &estBitrate);

    virtual RTPSink *
    createNewRTPSink(Groupsock *rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic,
                     FramedSource *inputSource);

    virtual char const *getAuxSDPLine(RTPSink *rtpSink, FramedSource *inputSource);
};


#endif //LIVERTSPSTREAMER_UNICASTSERVERMEDIASUBSESSION_H
