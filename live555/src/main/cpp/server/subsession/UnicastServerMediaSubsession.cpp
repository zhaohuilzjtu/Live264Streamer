//
// Created by huzongyao on 2019/4/30.
//

#include "UnicastServerMediaSubsession.h"

UnicastServerMediaSubsession *
UnicastServerMediaSubsession::createNew(UsageEnvironment &env, StreamReplicator *replicator) {
    return new UnicastServerMediaSubsession(env, replicator);
}

FramedSource *UnicastServerMediaSubsession::createNewStreamSource(unsigned clientSessionId,
                                                                  unsigned &estBitrate) {
    return createSource(envir(), mReplicator->createStreamReplica());
}

RTPSink *UnicastServerMediaSubsession::createNewRTPSink(Groupsock *rtpGroupsock,
                                                        unsigned char rtpPayloadTypeIfDynamic,
                                                        FramedSource *inputSource) {
    return createSink(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}

char const *
UnicastServerMediaSubsession::getAuxSDPLine(RTPSink *rtpSink, FramedSource *inputSource) {
    return this->getAuxLine((ByteArrayFramedSource *) (mReplicator->inputSource()),
                            rtpSink->rtpPayloadType());
}
