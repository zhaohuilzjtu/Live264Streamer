//
// Created by huzongyao on 2019/4/30.
//

#include "MulticastServerMediaSubsession.h"

MulticastServerMediaSubsession *
MulticastServerMediaSubsession::createNew(UsageEnvironment &env, struct in_addr destinationAddress,
                                          Port rtpPortNum, Port rtcpPortNum, int ttl,
                                          StreamReplicator *replicator) {
    // Create a source
    FramedSource *source = replicator->createStreamReplica();
    FramedSource *videoSource = createSource(env, source);

    // Create RTP/RTCP groupsock
    Groupsock *rtpGroupsock = new Groupsock(env, destinationAddress, rtpPortNum, (u_int8_t) ttl);
    Groupsock *rtcpGroupsock = new Groupsock(env, destinationAddress, rtcpPortNum, (u_int8_t) ttl);

    // Create a RTP sink
    RTPSink *videoSink = createSink(env, rtpGroupsock, 96);

    // Create 'RTCP instance'
    const unsigned maxCNAMElen = 100;
    unsigned char CNAME[maxCNAMElen + 1];
    gethostname((char *) CNAME, maxCNAMElen);
    CNAME[maxCNAMElen] = '\0';
    RTCPInstance *rtcpInstance =
            RTCPInstance::createNew(env, rtcpGroupsock, 500, CNAME, videoSink, NULL);

    // Start Playing the Sink
    videoSink->startPlaying(*videoSource, NULL, NULL);

    return new MulticastServerMediaSubsession(replicator, videoSink, rtcpInstance);
}

char const *MulticastServerMediaSubsession::sdpLines() {
    if (mSDPLines.empty()) {
        // Ugly workaround to give SPS/PPS that are get from the RTPSink
        mSDPLines.assign(PassiveServerMediaSubsession::sdpLines());
        mSDPLines.append(getAuxSDPLine(mRtpSink, NULL));
    }
    return mSDPLines.c_str();
}

char const *
MulticastServerMediaSubsession::getAuxSDPLine(RTPSink *rtpSink, FramedSource *inputSource) {
    return this->getAuxLine((ByteArrayFramedSource *) (mReplicator->inputSource()),
                            rtpSink->rtpPayloadType());
}
