package com.vantacom.aarm.libraries;


import android.annotation.SuppressLint;
import android.util.Log;

import com.vantacom.aarm.wine.WineActivity;

import java.io.File;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;

public class User32Lib implements Library {
    public User32 INSTANCE;
    WineActivity activity;

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public User32Lib(WineActivity activity, File libraryDir) {
        this.activity = activity;
        System.load(libraryDir.toString() + "/wine/user32.dll.so");
    }

    public User32Lib() {}

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public void init(WineActivity activity, File libraryDir) {
        this.activity = activity;
        Log.e("log", String.valueOf(Platform.isWindows()));
        INSTANCE =  Native.loadLibrary(libraryDir.toString() + "/wine/user32.dll.so", User32.class);
//        System.load(libraryDir.toString() + "/wine/user32.dll.so");
    }

    public void destroy() throws Throwable {
        activity = null;
    }

    interface User32 extends Library {
        public void keybd_event(byte bVk, byte bScan, int dwFlags, int dwExtraInfo);
    }

    public void keybd_event() {
        INSTANCE.keybd_event((byte) 0x41, (byte) 0x00, 0, 0); // a key_down
        INSTANCE.keybd_event((byte) 0x41, (byte) 0x00, 0x0002, 0); // a key_up
    }
}