package com.hzy.rtsp.live555;

public class Live555Api {
    static {
        System.loadLibrary("live555");
    }

    public static native String getVersionInfo();

    public static native long createNew();

    public static native int addUserRecord(long instanceId, String userName, String password);

    public static native int startServer(long instanceId, int port, String streamName);

    public static native String getStreamUrl(long instanceId);

    public static native int doEventLoop(long instanceId);

    public static native int feedH264Data(long instanceId, byte[] data);

    public static native int stopServer(long instanceId);

}
