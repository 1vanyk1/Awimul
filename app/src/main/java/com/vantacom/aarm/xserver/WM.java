package com.vantacom.aarm.xserver;

public class WM {
    static {
        System.loadLibrary("xserver");
    }

    public native int init(String path, String tmp);

    public native boolean checkXServerIsLoaded(String tmp);

    public native int startWM(String tmp);
}
