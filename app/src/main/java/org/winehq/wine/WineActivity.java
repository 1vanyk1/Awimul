package org.winehq.wine;

import android.annotation.SuppressLint;
import android.graphics.Rect;
import android.view.Surface;

import androidx.fragment.app.Fragment;

import com.vantacom.aarm.wine.WineService;

import java.io.File;

public class WineActivity extends Fragment {
    WineIStream oStream;

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public WineActivity(WineService oStream, File libraryDir) {
        this.oStream = oStream;
        try {
            System.loadLibrary("wine");
        } catch (UnsatisfiedLinkError e) {
            System.load(libraryDir.toString() + "/libwine.so");
        }
    }

    public WineActivity() {}

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public void init(WineService oStream, File libraryDir) {
        this.oStream = oStream;
        try {
            System.loadLibrary("wine");
        } catch (UnsatisfiedLinkError e) {
            System.load(libraryDir.toString() + "/libwine.so");
        }
    }

    @Override
    public void finalize() throws Throwable {
        super.finalize();
    }

    public native String wine_init(String[] wineParams, String[] wineSettings);

    public native void wine_config_changed(int dpi);

    public native void wine_desktop_changed(int width, int height);

    public native boolean wine_keyboard_event(int hwnd, int event, int key, int metaState);

    public native boolean wine_motion_event(int hwnd, int event, int x, int y, int buttonState, int scroll);

    public native void wine_surface_changed(int hwnd, Surface surface, boolean isClient);

    public void createDesktopWindow(int hwnd) {
        oStream.createDesktopWindow(hwnd);
        oStream.onWineLoad();
    }

    public void createWindow(int hwnd, boolean isClient, int parent, float scale, int i) {
        oStream.createWindow(hwnd, isClient, parent, scale);
    }

    public void windowPosChanged(int hwnd, int vis, int next_hwnd, int owner, int style, int win_l, int win_t, int win_r, int win_b, int client_l, int client_t, int client_r, int client_b, int vis_l, int vis_t, int vis_r, int vis_b) {
        Rect win_rect = new Rect(win_l, win_t, win_r, win_b);
        Rect client_rect = new Rect(client_l, client_t, client_r, client_b);
        Rect visible_rect = new Rect(vis_l, vis_t, vis_r, vis_b);
        oStream.windowPosChanged(hwnd, vis, next_hwnd, owner, style, win_rect, client_rect, visible_rect);
    }

    public void destroyWindow(int hwnd) {
        oStream.destroyWindow(hwnd);
    }

    public void setParent(int hwnd, int hwnd_parent, float scale, int int1) {
        oStream.setWindowParent(hwnd, hwnd_parent, scale);
    }

    public void setCursor(int int1, int int2, int int3, int int4, int int5, int[] int_array)  {
//        TODO
    }

    public void destroy() throws Throwable {
        oStream = null;
    }
}
