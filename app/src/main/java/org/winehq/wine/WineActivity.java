package org.winehq.wine;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Rect;
import android.util.Log;
import android.view.Surface;

import com.vantacom.aarm.libraries.Library;

import java.io.File;

public class WineActivity extends Activity {
    com.vantacom.aarm.wine.WineActivity activity;

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public WineActivity(com.vantacom.aarm.wine.WineActivity activity, File libraryDir) {
        this.activity = activity;
        try {
            System.loadLibrary("wine");
        } catch (UnsatisfiedLinkError e) {
            System.load(libraryDir.toString() + "/libwine.so");
        }
    }

    public WineActivity() {}

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public void init(com.vantacom.aarm.wine.WineActivity activity, File libraryDir) {
        this.activity = activity;
        try {
            System.loadLibrary("wine");
        } catch (UnsatisfiedLinkError e) {
            System.load(libraryDir.toString() + "/libwine.so");
        }
    }

    @Override
    public void finalize() throws Throwable {
        Log.e("wine", "WineActivity was finalized");
        super.finalize();
    }

    public native String wine_init(String[] wineParams, String[] wineSettings);

    public native void wine_config_changed(int dpi);

    public native void wine_desktop_changed(int width, int height);

    public native boolean wine_keyboard_event(int hwnd, int event, int key, int metaState);

    public native boolean wine_motion_event(int hwnd, int event, int x, int y, int buttonState, int scroll);

    public native void wine_surface_changed(int hwnd, Surface surface, boolean isClient);

    public void createDesktopWindow(int hwnd) {
        activity.createDesktopWindow(hwnd);
        activity.onWineLoad();
    }

    public void createWindow(int hwnd, boolean isClient, int paramInt2, float scale, int i) {
        activity.createWindow(hwnd, isClient, paramInt2, scale);
    }

    public void windowPosChanged(int hwnd, int vis, int next_hwnd, int owner, int style, int win_l, int win_t, int win_r, int win_b, int client_l, int client_t, int client_r, int client_b, int vis_l, int vis_t, int vis_r, int vis_b) {
        Rect win_rect = new Rect(win_l, win_t, win_r, win_b);
        Rect client_rect = new Rect(client_l, client_t, client_r, client_b);
        Rect visible_rect = new Rect(vis_l, vis_t, vis_r, vis_b);
        activity.windowPosChanged(hwnd, vis, next_hwnd, owner, style, win_rect, client_rect, visible_rect);
    }

    public void destroyWindow(int hwnd) {
        activity.destroyWindow(hwnd);
    }

    public void setParent(int hwnd, int hwnd_parent, float scale, int int1) {
        activity.setWindowParent(hwnd, hwnd_parent, scale);
    }

    public void setCursor(int int1, int int2, int int3, int int4, int int5, int[] int_array)  {
//        TODO
    }

    public void destroy() throws Throwable {
        activity = null;
        this.finish();
    }
}
