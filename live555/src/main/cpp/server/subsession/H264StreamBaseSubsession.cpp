//
// Created by huzongyao on 2019/4/30.
//

#include <include/H264VideoStreamDiscreteFramer.hh>
#include <include/H264VideoRTPSink.hh>
#include <sstream>
#include "H264StreamBaseSubsession.h"

FramedSource *
H264StreamBaseSubsession::createSource(UsageEnvironment &env, FramedSource *videoES) {
    return H264VideoStreamDiscreteFramer::createNew(env, videoES);;
}

RTPSink *H264StreamBaseSubsession::createSink(UsageEnvironment &env, Groupsock *rtpGroupsock,
                                              unsigned char rtpPayloadTypeIfDynamic) {
    return H264VideoRTPSink::createNew(env, rtpGroupsock, rtpPayloadTypeIfDynamic);;
}

char const *
H264StreamBaseSubsession::getAuxLine(ByteArrayFramedSource *source,
                                     unsigned char rtpPayloadType) {
    const char *auxLine = NULL;
    if (source) {
        std::ostringstream os;
        os << "a=fmtp:" << int(rtpPayloadType) << " ";
        os << source->getAuxLine();
        os << "\r\n";
        auxLine = strdup(os.str().c_str());
    }
    return auxLine;
}
