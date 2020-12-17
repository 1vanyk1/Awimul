package com.vantacom.aarm.wine;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Rect;
import android.os.Bundle;
import android.util.Log;

import com.vantacom.aarm.R;
import com.vantacom.aarm.wine.views.Window;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;

public class WineActivity extends Activity {
    private org.winehq.wine.WineActivity wineActivity;
    private File filesDir;
    private MainView mainView;
    private HashMap<Integer, Window> windowsHM = new HashMap<Integer, Window>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wine);
        wineActivity = new org.winehq.wine.WineActivity(this);
        filesDir = getFilesDir();
        new Thread(new Runnable() {
            public void run() {
                loadWine(null);
            }
        }).start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        wineActivity.destroy();
        Window[] windowsArray = windowsHM.values().toArray(new Window[0]);
        for (int i = 0; i < windowsHM.values().size(); i++) {
            windowsArray[i].destroy();
        }
        windowsHM.clear();
        mainView.destroy();
        mainView = null;
        try {
            Runtime.getRuntime().exec("wineserver -k9");
        } catch (IOException e) {
            Log.e("WA/onDestroy", e.toString());
        }
    }

    public Window getWindow(int hwnd) {
        return windowsHM.get(hwnd);
    }

    public void addWindow(int hwnd, Window window) {
        windowsHM.put(hwnd, window);
    }

    public void removeWindow(Window window) {
        windowsHM.remove(window);
    }

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public void loadWine(String path2file) {
        String wineABI = "armeabi-v7a";
        File binDir = new File(filesDir, wineABI + "/bin");
        File libraryDir = new File(filesDir, wineABI + "/lib");
        File winePrefix = new File(filesDir, "prefix");
        String[] wineSettings = StartupManager.getWineSetting(binDir, libraryDir, winePrefix, getApplicationInfo());
        if (path2file == null) {
            path2file = StartupManager.getFilePath(winePrefix, "cmd");
        }
        try {
            System.loadLibrary("wine");
        }
        catch (UnsatisfiedLinkError e) {
            Log.e("WA", e.toString());
            System.load(libraryDir.toString() + "/libwine.so");
        }
        winePrefix.mkdirs();
        for (int i = 0; i < wineSettings.length; i += 2) {
            Log.i("WA/wineSettings", String.format("%s: %s", wineSettings[i], wineSettings[i + 1]));
        }
        wineActivity.runWine(path2file, wineSettings);
    }

    public void createDesktopWindow(int desktopView) {
        runOnUiThread(new Runnable() {
            public void run() {
                mainView = new MainView(WineActivity.this, wineActivity, WineActivity.this, desktopView);
                setContentView(mainView);
                wineActivity.wine_config_changed(getResources().getConfiguration().densityDpi);
            }
        });
    }

    public void createWindow(int hwnd, boolean isClient, int parent, float scale) {
        runOnUiThread(new Runnable() {
            public void run() {
                Window window = getWindow(hwnd);
                if (window == null) {
                    window = new Window(WineActivity.this, wineActivity, hwnd, getWindow(parent), scale);
                    window.createWindowGroups();
                    if (window.getParent() == mainView.getDesktopWindow()) {
                        window.createWindowView();
                    }
                }
                if (isClient) {
                    window.createClientView();
                }
            }
        });
    }

    public void destroyWindow(int hwnd) {
        runOnUiThread(new Runnable() {
            public void run() {
                Window wineWindow = getWindow(hwnd);
                if (wineWindow != null) {
                    wineWindow.destroy();
                }
            }
        });
    }

    public void setWindowParent(int hwnd, int hwnd_parent, float scale) {
        runOnUiThread(new Runnable() {
            public void run() {
                Window window = getWindow(hwnd);
                if (window != null) {
                    window.setParent(getWindow(hwnd_parent), scale);
                    if (window.getParent() == mainView.getDesktopWindow()) {
                        window.createWindowView();
                    }
                }
            }
        });
    }

    public void windowPosChanged(int hwnd, int vis, int next_hwnd, int owner, int style, Rect win_rect, Rect client_rect, Rect visible_rect) {
        runOnUiThread(new Runnable() {
            public void run() {
                Window window = getWindow(hwnd);
                if (window != null) {
                    window.posChanged(vis, next_hwnd, owner, style, client_rect, visible_rect);
                }
            }
        });
    }
}
