package com.vantacom.aarm.ipc;

public class IPClib {
    static {
        System.loadLibrary("ipc");
    }

    public native String string();

    public native void init(String string);

    public native void sendSignal(int pid);
}
