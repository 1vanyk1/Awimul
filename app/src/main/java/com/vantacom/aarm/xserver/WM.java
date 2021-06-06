package com.vantacom.aarm.xserver;

public class WM {
    static {
        System.loadLibrary("xserver");
    }

    public native int init(String path);
}
