package com.vantacom.aarm.xserver;

import android.annotation.SuppressLint;
import android.util.Log;
import android.view.Surface;

import com.vantacom.aarm.wine.WineService;

public class WM {
    private WineService oStream;

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public WM(WineService oStream) {
        this.oStream = oStream;
        System.loadLibrary("xserver");
    }

    public WM() {
        System.loadLibrary("xserver");
    }

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public void init(WineService oStream) {
        this.oStream = oStream;
        System.loadLibrary("xserver");
    }

//    static {
//        System.loadLibrary("xserver");
//    }

    public native int init(String tmp, int width, int height);

    public native void setSurface(Surface surface);

    public native void shutDown();

    public native void pauseServer(boolean pause);

    public native void mouseEvent(int action, int x, int y, int button);

    public native void keyPress(int key, int isPressed);

    public void loadEnd() {
        if (oStream != null) {
            Log.e("TRUE!", "TRUE!");
            oStream.createDesktopWindow(1);
            oStream.onWineLoad();
        }
    }
}
