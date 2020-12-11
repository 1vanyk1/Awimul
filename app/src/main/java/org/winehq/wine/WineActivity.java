package org.winehq.wine;

import android.app.Activity;
import android.util.Log;
import android.view.Surface;

public class WineActivity extends Activity {
    public void runWine(String path2cmd, String[] wineSettings) {
        Log.e("wine", "2");
        Log.e("wine", wine_init(new String[]{wineSettings[1], "explorer.exe", "/desktop=shell,,android", path2cmd}, wineSettings));
    }

    public native String wine_init(String[] wineParams, String[] wineSettings);

    public native void wine_config_changed(int int1);

    public native void wine_desktop_changed(int int1, int int2);

    public native boolean wine_keyboard_event(int int1, int int2, int int3, int int4);

    public native boolean wine_motion_event(int int1, int int2, int int3, int int4, int int5, int int6);

    public native void wine_surface_changed(int int1, Surface surface, boolean bool1);

    public void createDesktopWindow(int int1) {
        Log.e("wine", "createWindow");
//        TODO
    }

    public void createWindow(int int1, boolean bool1, int int2, float float1, int int3) {
//        TODO
    }

    public void windowPosChanged(int int1, int int2, int int3, int int4, int int5, int int6, int int7, int int8, int int9, int int10, int int11, int int12, int int13, int int14, int int15, int int16, int int17) {
//        TODO
    }

    public void destroyWindow(int int1) {
//        TODO
    }
}
