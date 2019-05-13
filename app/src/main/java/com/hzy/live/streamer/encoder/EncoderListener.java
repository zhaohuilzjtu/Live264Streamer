package com.hzy.live.streamer.encoder;

public interface EncoderListener {
    /**
     * To invoke if output buffer is availiable
     *
     * @param output encoder output data
     */
    void onEncodeRaw(byte[] output);
}
