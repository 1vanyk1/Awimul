 package com.vantacom.aarm.wine;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.sun.jna.CallbackThreadInitializer;
import com.sun.jna.PointerType;
import com.sun.jna.Structure;
import com.sun.jna.win32.StdCallLibrary;
import com.sun.jna.win32.W32APIOptions;
import com.vantacom.aarm.LibraryManager;
import com.vantacom.aarm.R;
import com.vantacom.aarm.dialogs.ConfirmTurnOff;
import com.vantacom.aarm.dialogs.LoadingWineDialog;
import com.vantacom.aarm.libraries.jna.User32;
import com.vantacom.aarm.libraries.jna.WinDef;
import com.vantacom.aarm.managers.ConsoleManager;
import com.vantacom.aarm.managers.FileManager;
import com.vantacom.aarm.managers.ProcessManager;
import com.vantacom.aarm.managers.SaveFilesManager;
import com.vantacom.aarm.wine.controls.Controls;
import com.vantacom.aarm.wine.xserver.XServerManager;

import java.io.File;


import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Platform;
import com.sun.jna.Pointer;


public class WineActivity extends AppCompatActivity implements View.OnTouchListener {
    private File filesDir;
    private ConstraintLayout view;
    private String wineABI;
    private LibraryManager wineActivity, user32;
    private ProcessManager processManager;
    private SaveFilesManager saveFilesManager;
    private XServerManager xserver;

    private View keyboard, exit;

    private View touchView, topPanel;

    private boolean ifTurnedOff = false;

    private LoadingWineDialog dialog;

    private void hideSystemUI() {
        getWindow().getDecorView().setSystemUiVisibility( View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

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
        xserver.getKeyboard().hideKeyboard();
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
        String pwd = FileManager.fixPWD(this, ConsoleManager.runCommandWithLog("pwd"));
        if (pwd.charAt(pwd.length() - 1) != '/') {
            pwd = pwd + '/';
        }
        if (saveFilesManager.getIsFirstLoad()) {
            ConsoleManager.runCommand(String.format("ln -s %s " + FileManager.getPrefixPath(this, "prefix") + "/dosdevices/d:", Environment.getExternalStorageDirectory().getPath()));
            FileManager.createFile(pwd + "logpixels.reg",
                    "REGEDIT4\n" +
                    "\n" +
                    "[HKEY_CURRENT_USER\\Control Panel\\Desktop]\n" +
                    "\"LogPixels\"=dword:00000060\n" +
                    "\n" +
                    "[HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Hardware Profiles\\Current\\Software\\Fonts]\n" +
                    "\"LogPixels\"=dword:00000060");
            ConsoleManager.runCommand("wine regedit logpixels.reg");
            FileManager.deleteFile(pwd + "logpixels.reg");
            saveFilesManager.setIsFirstLoad(false);
        }
        runOnUiThread(new Runnable() {
            public void run() {
                topPanel.setVisibility(View.VISIBLE);
            }
        });
        try {
            user32 = new LibraryManager("com.vantacom.aarm.libraries.User32Lib");
            user32.invoke("init", this, new File(filesDir, wineABI + "/lib"));
        } catch (Exception e) {
            Log.e("WA", e.toString());
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_wine);
        saveFilesManager = new SaveFilesManager(this, "prefix");
        wineABI = "armeabi-v7a";
        filesDir = getFilesDir();

        view = findViewById(R.id.mainLayout);
        touchView = findViewById(R.id.touchView);
        topPanel = findViewById(R.id.topPanel);
        keyboard = findViewById(R.id.keyboard);
        keyboard.setOnTouchListener(this);
        exit = findViewById(R.id.exit);
        exit.setOnTouchListener(this);

        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        int screenWidth = displayMetrics.widthPixels;
        int screenHeight = displayMetrics.heightPixels;
        int desktopWidth = getIntent().getIntExtra("width", screenWidth);
        int desktopHeight = getIntent().getIntExtra("height", screenHeight);
        int w = screenWidth, h = screenHeight;
        if (desktopWidth > screenWidth) {
            w = desktopWidth;
        }
        if (desktopHeight > screenHeight) {
            h = desktopHeight;
        }
        view.setLayoutParams(new ConstraintLayout.LayoutParams(w, h));
        try {
            wineActivity = new LibraryManager("org.winehq.wine.WineActivity");
            wineActivity.invoke("init", this, new File(filesDir, wineABI + "/lib"));
        } catch (Exception e) {
            Log.e("WA", e.toString());
        }
        xserver = new XServerManager(screenWidth, screenHeight, desktopWidth, desktopHeight, this, wineActivity);
        touchView.setOnTouchListener(new Controls(this, xserver));
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
        view.removeAllViews();
        xserver.destroy();

        ConsoleManager.runCommand("wineserver -k");
        wineActivity.destroy();
        wineActivity = null;
        ifTurnedOff = true;
        finish();
    }

//    public interface User32 extends Library {
//        public void keybd_event(byte bVk, byte bScan, int dwFlags, int dwExtraInfo);
//        public int GetWindow(int hwnd, int rel);
//        public boolean IsWindowVisible(int hwnd);
//    }
//
//    public interface Kernel32 extends StdCallLibrary {
//        Kernel32 INSTANCE = (Kernel32) Native.load("/data/user/0/com.vantacom.aarm/files/armeabi-v7a/lib/wine/kernel32.dll.so", Kernel32.class, W32APIOptions.DEFAULT_OPTIONS);
//        Kernel32 SYNC_INSTANCE = (Kernel32) Native.synchronizedLibrary(INSTANCE);
//
//        @Structure.FieldOrder({ "wYear", "wMonth", "wDayOfWeek", "wDay", "wHour", "wMinute", "wSecond", "wMilliseconds" })
//        public static class SYSTEMTIME extends Structure {
//            public short wYear;
//            public short wMonth;
//            public short wDayOfWeek;
//            public short wDay;
//            public short wHour;
//            public short wMinute;
//            public short wSecond;
//            public short wMilliseconds;
//        }
//
//        void GetSystemTime(SYSTEMTIME result);
//    }

    @Override
    public void onBackPressed() {
        hideSystemUI();
        if (xserver.getDesktopView() != null && !isSystemPaused()) {
//            user32.invoke("keybd_event", (byte)0x31, (byte)0x2, 0, 0);
//            user32.invoke("keybd_event", (byte)0x31, (byte)0x02, 0x0002, 0);

            Log.e(String.valueOf(getFilesDir()), "1");

//            try {
//                Kernel32 lib = Kernel32.INSTANCE;
//                Kernel32.SYSTEMTIME time = new Kernel32.SYSTEMTIME();
//                lib.GetSystemTime(time);
//
//                System.out.println("Today's integer value is " + time.wDay);
//            } catch (Exception e) {
//                Log.e("e", e.toString());
//            }


//            User32 lib = Native.loadLibrary(new File(filesDir, wineABI + "/lib/wine/user32.dll.so").toString(), User32.class);
//            Log.e("1", "2");
////            lib.test_keyboard_events();
////            Log.e("1", "2");
//            lib.IsWindowVisible(xserver.getFocusedWindow().getHWND());
////            lib.GetWindow(xserver.getFocusedWindow().getHWND(), xserver.getDesktopView().getDesktopWindow().getHWND());
//            Log.e("1", "2");


//            lib.keybd_event((byte)0x31, (byte)0x02, 0, 0);
//            lib.keybd_event((byte)0x31, (byte)0x02, 0x0002, 0);
//            Log.e("Key", "ONBACK");
//            user32.invoke("keybd_event");
//            Log.e("Key", "ONBACK");
//            user32.invoke("keybd_event");
//            Log.e("Key", "ONBACK");

//            xserver.getKeyboard().pressKey(0, 111, 0);
//            xserver.getKeyboard().pressKey(1, 111, 0);
        }
    }

    public interface User32_1 extends Library {
        User32_1 INSTANCE = (User32_1) Native.loadLibrary("/data/user/0/com.vantacom.aarm/files/armeabi-v7a/lib/wine/user32.dll.so", User32_1.class);
        WinDef.HWND GetForegroundWindow();  // add this
        int GetWindowTextA(PointerType hWnd, byte[] lpString, int nMaxCount);
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        Log.e("1", "!");
        User32 user32 = User32.INSTANCE;
        WinDef.HWND hwnd = user32.GetDesktopWindow();
//        WinDef.HWND fgWindow = User32_1.INSTANCE.GetForegroundWindow();
        if (!isSystemPaused()) {
            boolean b = xserver.getKeyboard().pressKey(event);
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

    public void createDesktopWindow(int hwnd) {
        runOnUiThread(new Runnable() {
            public void run() {
                if (dialog.isShowing()) {
                    dialog.dismiss();
                }
                xserver.createDesktopWindow(hwnd);
                view.addView(xserver.getDesktopView());
            }
        });
    }

    public void createWindow(int hwnd, boolean isClient, int parent, float scale) {
        runOnUiThread(new Runnable() {
            public void run() {
                xserver.createWindow(hwnd, isClient, parent, scale);
            }
        });
    }

    public void destroyWindow(int hwnd) {
        runOnUiThread(new Runnable() {
            public void run() {
                xserver.destroyWindow(hwnd);
            }
        });
    }

    public void setWindowParent(int hwnd, int hwnd_parent, float scale) {
        runOnUiThread(new Runnable() {
            public void run() {
                xserver.setWindowsParent(hwnd, hwnd_parent, scale);
            }
        });
    }

    public void windowPosChanged(int hwnd, int vis, int next_hwnd, int owner, int style, Rect win_rect, Rect client_rect, Rect visible_rect) {
        runOnUiThread(new Runnable() {
            public void run() {
                xserver.windowPosChanged(hwnd, vis, next_hwnd, owner, style, win_rect, client_rect, visible_rect);
            }
        });
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            if (v.getId() == R.id.keyboard) {
                xserver.getKeyboard().toggleKeyboard();
            } else if (v.getId() == R.id.exit) {
                ConfirmTurnOff dialog = new ConfirmTurnOff(this);
                FragmentManager manager = getSupportFragmentManager();
                FragmentTransaction transaction = manager.beginTransaction();
                dialog.show(transaction, "dialog");
            }
        }
        return true;
    }

    public void toggleTopBar() {
        if (topPanel.getVisibility() == View.VISIBLE) {
            topPanel.setVisibility(View.INVISIBLE);
        } else {
            topPanel.setVisibility(View.VISIBLE);
        }
    }
}
