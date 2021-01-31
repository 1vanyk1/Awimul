 package com.vantacom.aarm.wine;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;

import androidx.constraintlayout.widget.ConstraintLayout;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.R;
import com.vantacom.aarm.dialogs.LoadingWineDialog;
import com.vantacom.aarm.managers.ConsoleManager;
import com.vantacom.aarm.managers.FileManager;
import com.vantacom.aarm.managers.ProcessManager;
import com.vantacom.aarm.managers.SaveFilesManager;
import com.vantacom.aarm.wine.controls.Keyboard;
import com.vantacom.aarm.wine.views.Window;

import java.io.File;
import java.util.HashMap;

public class WineActivity extends Activity {
    private File filesDir;
    private MainView mainView;
    private ConstraintLayout view;
    private HashMap<Integer, Window> windowsHM = new HashMap<Integer, Window>();
    private String wineABI;
    private CustomClassManager wineActivity;
    private Keyboard keyboard;
    private ProcessManager processManager;
    private SaveFilesManager saveFilesManager;

    private boolean ifTurnedOff = false;

    private int screenWidth, screenHeight;
    private int desktopWidth, desktopHeight;

    private LoadingWineDialog dialog;

    private void hideSystemUI() {
        getWindow().getDecorView().setSystemUiVisibility( View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY );
    }

    public int getScreenWidth() {
        return screenWidth;
    }

    public int getScreenHeight() {
        return screenHeight;
    }

    public int getDesktopWidth() {
        return desktopWidth;
    }

    public int getDesktopHeight() {
        return desktopHeight;
    }

    public Keyboard getKeyboard() { return keyboard; }

    public MainView getMainView() { return mainView; }

    public Window getWindow(int hwnd) { return windowsHM.get(hwnd); }

    public void addWindow(int hwnd, Window window) { windowsHM.put(hwnd, window); }

    public void removeWindow(Window window) { windowsHM.remove(window); }

    public boolean isSystemPaused() {
        if (processManager == null || !processManager.getIsPaused()) {
            return false;
        }
        return true;
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (processManager != null) {
            processManager.pauseSystem();
        }
        keyboard.hideKeyboard();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (processManager != null) {
            processManager.resumeSystem();
        }
        hideSystemUI();
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemUI();
        }
    }

    public void onWineLoad() {
        processManager = new ProcessManager();
        ConsoleManager.runCommand(String.format("ln -s %s ../dosdevices/d:", Environment.getExternalStorageDirectory().getPath()));
        if (saveFilesManager.getIsFirstLoad()) {
            Log.e("1", FileManager.getDriveCPath(this, "prefix") + "/logpixels.reg");
            FileManager.createFile(FileManager.getDriveCPath(this, "prefix") + "/logpixels.reg",
                    "REGEDIT4\n" +
                    "\n" +
                    "[HKEY_CURRENT_USER\\Control Panel\\Desktop]\n" +
                    "\"LogPixels\"=dword:00000060\n" +
                    "\n" +
                    "[HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Hardware Profiles\\Current\\Software\\Fonts]\n" +
                    "\"LogPixels\"=dword:00000060");
            ConsoleManager.runCommand("wine regedit logpixels.reg");
            FileManager.deleteFile(FileManager.getDriveCPath(this, "prefix") + "/logpixels.reg");
            saveFilesManager.setIsFirstLoad(false);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wine);
        saveFilesManager = new SaveFilesManager(this, "prefix");
        keyboard = new Keyboard(this);
        wineABI = "armeabi-v7a";
        filesDir = getFilesDir();
        view = findViewById(R.id.mainLayout);
        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        screenWidth = displayMetrics.widthPixels;
        screenHeight = displayMetrics.heightPixels;
        desktopWidth = getIntent().getIntExtra("width", screenWidth);
        desktopHeight = getIntent().getIntExtra("height", screenHeight);
        int w = screenWidth, h = screenHeight;
        if (desktopWidth > screenWidth) {
            w = desktopWidth;
        }
        if (desktopHeight > screenHeight) {
            h = desktopHeight;
        }
        view.setLayoutParams(new ConstraintLayout.LayoutParams(w, h));
        try {
            wineActivity = new CustomClassManager("org.winehq.wine.WineActivity");
            wineActivity.invoke("init", this, new File(filesDir, wineABI + "/lib"));
        } catch (Exception e) {
            Log.e("WA", e.toString());
        }
        dialog = new LoadingWineDialog(this);
        dialog.show();
        hideSystemUI();
        new Thread(new Runnable() {
            public void run() {
                loadWine(null);
            }
        }).start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (!ifTurnedOff) {
            onTurnOff();
        }
    }

    public void onTurnOff() {
        keyboard.hideKeyboard();
        Window[] windowsArray = windowsHM.values().toArray(new Window[0]);
        for (int i = 0; i < windowsHM.values().size(); i++) {
            windowsArray[i].destroy();
        }
        windowsHM.clear();
        view.removeAllViews();
        mainView.destroy();
        mainView = null;

        ConsoleManager.runCommand("wineserver -k");
        wineActivity.destroy();
        wineActivity = null;
        ifTurnedOff = true;
        finish();
    }

    @Override
    public void onBackPressed() {
        hideSystemUI();
        if (mainView != null && !isSystemPaused()) {
            wineActivity.invoke("wine_keyboard_event", keyboard.getHWND(), 0, 111, 0);
            wineActivity.invoke("wine_keyboard_event", keyboard.getHWND(), 1, 111, 0);
        }
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if (!isSystemPaused()) {
            boolean b = (boolean) wineActivity.invoke("wine_keyboard_event", keyboard.getHWND(), event.getAction(), event.getKeyCode(), event.getMetaState());
            if (!b) {
                b = super.dispatchKeyEvent(event);
            }
            return b;
        }
        return super.dispatchKeyEvent(event);
    }

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public void loadWine(String path2file) {
        File binDir = new File(filesDir, wineABI + "/bin");
        File libraryDir = new File(filesDir, wineABI + "/lib");
        File winePrefix = new File(filesDir, "prefix");
        String[] wineSettings = StartupManager.getWineSetting(binDir, libraryDir, winePrefix, getApplicationInfo());
        if (path2file == null) {
            path2file = StartupManager.getFilePath(winePrefix, "explorer");
        }
        winePrefix.mkdirs();
        for (int i = 0; i < wineSettings.length; i += 2) {
            Log.i("WA/wineSettings", String.format("%s: %s", wineSettings[i], wineSettings[i + 1]));
        }
        runWine(path2file, wineSettings);
    }

    private void runWine(String path2file, String[] wineSettings) {
        wineActivity.invoke("wine_init", new String[]{wineSettings[1], "explorer", "/desktop=shell,,android", path2file}, wineSettings);
    }

    public void createDesktopWindow(int desktopView) {
        runOnUiThread(new Runnable() {
            public void run() {
                if (dialog.isShowing()) {
                    dialog.dismiss();
                }
                mainView = new MainView(WineActivity.this, wineActivity, WineActivity.this, desktopView);
                view.addView(mainView);
                try {
                    wineActivity.invoke("wine_config_changed", getResources().getConfiguration().densityDpi);
                } catch (Exception e) {
                    Log.e("wine", e.toString());
                }
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
