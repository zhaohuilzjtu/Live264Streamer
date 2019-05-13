//
// Created by huzongyao on 2019/4/20.
//

#ifndef LIVERTSPSTREAMER_TOPSTREAMSERVER_H
#define LIVERTSPSTREAMER_TOPSTREAMSERVER_H


#include <jni.h>
#include <include/GenericMediaServer.hh>
#include <include/RTSPServer.hh>
#include "framesource/H264ArrayFramedSource.h"

class TopStreamServer {
public:
    static TopStreamServer *createNew();

    void addUserRecord(char const *userName, char const *password);

    int startServer(int port, const char *streamName);

    char *getStreamUrl();

    void doEventLoop();

    void feedH264Data(char *data, unsigned int dataSize);

    void stopServer();

    virtual ~TopStreamServer();

protected:
    TopStreamServer();

    UsageEnvironment *mEnv = NULL;
    UserAuthenticationDatabase *mAuthDB = NULL;
    RTSPServer *mRTSPServer = NULL;
    char volatile mWatchVariable = 1;
    TaskScheduler *mScheduler = NULL;
    H264ArrayFramedSource *mH264FramedSource = NULL;
    ServerMediaSession *mMediaSession = NULL;
    char *mStreamUrl = NULL;
};


#endif //LIVERTSPSTREAMER_TOPSTREAMSERVER_H
