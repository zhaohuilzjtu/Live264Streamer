package com.hzy.live.streamer.instance;

import com.hzy.rtsp.live555.Live555Api;

public enum Live555Instance implements Runnable {

    INSTANCE;

    private volatile long mInstanceId;
    private Thread mThread;
    private volatile boolean mIsRunning;

    /**
     * Auto create instance;
     */
    Live555Instance() {
        mInstanceId = Live555Api.createNew();
    }

    /**
     * Access Control
     * Call Before start()
     *
     * @param userName userName
     * @param password password
     */
    public void addUserRecord(String userName, String password) {
        Live555Api.addUserRecord(mInstanceId, userName, password);
    }

    public void start() {
        if (Live555Api.startServer(mInstanceId, 9554, "stream") == 0) {
            mThread = new Thread(this);
            mThread.start();
            mIsRunning = true;
        }
    }

    public boolean isIsRunning() {
        return mIsRunning;
    }

    public String getStreamUrl() {
        return Live555Api.getStreamUrl(mInstanceId);
    }

    public boolean feedH264Data(byte[] data) {
        return Live555Api.feedH264Data(mInstanceId, data) == 0;
    }

    public boolean stopServer() {
        return Live555Api.stopServer(mInstanceId) == 0;
    }

    private boolean doEventLoop() {
        return Live555Api.doEventLoop(mInstanceId) == 0;
    }

    /**
     * Do event loop on another java thread
     */
    @Override
    public void run() {
        doEventLoop();
        mIsRunning = false;
    }
}
