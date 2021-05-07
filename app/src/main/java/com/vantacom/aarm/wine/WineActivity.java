 package com.vantacom.aarm.wine;

import android.annotation.SuppressLint;
import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Environment;
import android.os.IBinder;
import android.os.Messenger;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.vantacom.aarm.BackgroundService;
import com.vantacom.aarm.LibraryManager;
import com.vantacom.aarm.R;
import com.vantacom.aarm.dialogs.ConfirmTurnOff;
import com.vantacom.aarm.dialogs.LoadingWineDialog;
import com.vantacom.aarm.managers.ConsoleManager;
import com.vantacom.aarm.managers.FileManager;
import com.vantacom.aarm.managers.PackageDBManager;
import com.vantacom.aarm.managers.ProcessManager;
import com.vantacom.aarm.wine.controls.touchscreen.BaseControls;
import com.vantacom.aarm.wine.controls.touchscreen.MouseControls;
import com.vantacom.aarm.wine.controls.touchscreen.TouchControls;
import com.vantacom.aarm.wine.xserver.XServerManager;

import java.io.File;


public class WineActivity extends AppCompatActivity implements View.OnTouchListener {
    private ConstraintLayout view;
    private String wineABI;
    private LibraryManager wineActivity;
    private ProcessManager processManager;
    private PackageDBManager sqLiteManager;
    private XServerManager xserver;
    private BaseControls controls;
    private String packageName;
    private boolean workInBG, showCursor;
    private String inputType;

    private ImageView keyboard, inputTypeView, exit;
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

    private boolean mShouldUnbind;

    private Messenger mService;

    private ServiceConnection mConnection;

    void doBindService() {
        if (bindService(new Intent(WineActivity.this, BackgroundService.class), mConnection, Context.BIND_AUTO_CREATE)) {
            mShouldUnbind = true;
        } else {
            Log.e("WA", "The requested service doesn't exist, or this client isn't allowed access to it.");
        }
    }

    void doUnbindService() {
        if (mShouldUnbind) {
            unbindService(mConnection);
            mShouldUnbind = false;
        }
    }

    private boolean isServiceRunning(Class<?> serviceClass) {
        ActivityManager manager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        for (ActivityManager.RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
            if (serviceClass.getName().equals(service.service.getClassName())) {
                return true;
            }
        }
        return false;
    }

    public boolean isSystemPaused() {
        if (processManager == null || !processManager.getIsPaused()) {
            return false;
        }
        return true;
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemUI();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        ifTurnedOff = false;
        if (processManager != null && !workInBG) {
            processManager.resumeSystem();
        }
        hideSystemUI();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent service = new Intent(this, BackgroundService.class);
        startService(service);
        mConnection = new ServiceConnection() {
            public void onServiceConnected(ComponentName className, IBinder service) {
                mService = new Messenger(service);
                Toast.makeText(WineActivity.this, "connected",
                        Toast.LENGTH_SHORT).show();
            }

            public void onServiceDisconnected(ComponentName className) {
                mService = null;
                Toast.makeText(WineActivity.this, "disconnected",
                        Toast.LENGTH_SHORT).show();
            }
        };
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        try {this.getSupportActionBar().hide();}
        catch (NullPointerException e) {}
        setContentView(R.layout.activity_wine);
        sqLiteManager = PackageDBManager.getInstance(this);
        packageName = getIntent().getStringExtra("package");
        workInBG = sqLiteManager.isBool(packageName, "workInBackground");
        showCursor = sqLiteManager.isBool(packageName, "showCursor");
        wineABI = "armeabi-v7a";

        view = findViewById(R.id.mainLayout);
        touchView = findViewById(R.id.touchView);
        topPanel = findViewById(R.id.topPanel);
        keyboard = findViewById(R.id.keyboard);
        keyboard.setOnTouchListener(this);
        inputTypeView = findViewById(R.id.input_type);
        inputTypeView.setOnTouchListener(this);
        exit = findViewById(R.id.exit);
        exit.setOnTouchListener(this);

        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        int screenWidth = displayMetrics.widthPixels;
        int screenHeight = displayMetrics.heightPixels;
        String size = sqLiteManager.getString(packageName, "size");
        int desktopWidth, desktopHeight;
        if (size.equals("native")) {
            desktopWidth = screenWidth;
            desktopHeight = screenHeight;
        } else {
            String[] sizes = size.split("x");
            desktopWidth = Integer.parseInt(sizes[0]);
            desktopHeight = Integer.parseInt(sizes[1]);
        }
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
            wineActivity.invoke("init", this, new File(getFilesDir(), wineABI + "/lib"));
        } catch (Exception e) {
            Log.e("WA", e.toString());
        }
        xserver = new XServerManager(screenWidth, screenHeight, desktopWidth, desktopHeight, this, wineActivity);
        changeInputType("touch");
        dialog = new LoadingWineDialog(this);
        dialog.show();
        hideSystemUI();
        doBindService();

        mYourService = new BackgroundService();
        Intent mServiceIntent = new Intent(this, mYourService.getClass());
        if (!isServiceRunning(mYourService.getClass())) {
            startService(mServiceIntent);
        }

        new Thread(new Runnable() {
            public void run() {
                loadWine(null, getFilesDir());
            }
        }).start();
    }

    public void onWineLoad() {
        processManager = new ProcessManager();
        String pwd = FileManager.fixPWD(this, ConsoleManager.runCommandWithLog("pwd"));
        if (pwd.charAt(pwd.length() - 1) != '/') {
            pwd = pwd + '/';
        }
        if (sqLiteManager.isBool(packageName, "firstLoad")) {
            File file = new File(Environment.getExternalStorageDirectory().getPath() + "/Awimul");
            if (!file.exists()) file.mkdir();
            ConsoleManager.runCommand(String.format("ln -s %s " + FileManager.getPrefixPath(this, "prefixes/" + sqLiteManager.getString(packageName, "prefix")) + "/dosdevices/d:", Environment.getExternalStorageDirectory().getPath() + "/Awimul"));
            sqLiteManager.setBool(packageName, "firstLoad", false);
        }
        runOnUiThread(new Runnable() {
            public void run() {
                topPanel.setVisibility(View.VISIBLE);
            }
        });
    }

    @Override
    protected void onPause() {
        super.onPause();
        ifTurnedOff = true;
        if (processManager != null && !workInBG) {
            processManager.pauseSystem();
        }
        xserver.getKeyboard().hideKeyboard();
    }

    private BackgroundService mYourService;

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (!ifTurnedOff) {
            onTurnOff();
        }
    }

    public void onTurnOff() {
        doUnbindService();
        Intent service = new Intent(this, BackgroundService.class);
        stopService(service);
        view.removeAllViews();
        xserver.destroy();
        ConsoleManager.runCommand("wineserver -k");
        wineActivity.destroy();
        wineActivity = null;
        ifTurnedOff = true;
        finish();
    }

    @Override
    public void onBackPressed() {
        hideSystemUI();
        if (xserver.getDesktopView() != null && !isSystemPaused()) {
            xserver.getKeyboard().pressKey(0, 111, 0);
            xserver.getKeyboard().pressKey(1, 111, 0);
        }
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
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
    public void loadWine(String path2file, File filesDir) {
        File binDir = new File(filesDir, wineABI + "/bin");
        File libraryDir = new File(filesDir, wineABI + "/lib");
        File winePrefix = new File(filesDir, "prefixes/" + sqLiteManager.getString(packageName, "prefix"));
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
            } else if (v.getId() == R.id.input_type) {
                if (inputType.equals("touch")) {
                    changeInputType("mouse");
                } else {
                    changeInputType("touch");
                }
            }
        }
        return true;
    }

    private void changeInputType(String inputType) {
        this.inputType = inputType;
        if (controls != null) {
            controls.stopMoving();
        }
        if (inputType.equals("mouse")) {
            xserver.getCursor().setVisibility(true);
            inputTypeView.setImageResource(R.drawable.mouse);
            controls = new MouseControls(this, xserver);
        } else {
            if (showCursor) {
                xserver.getCursor().setVisibility(true);
            } else {
                xserver.getCursor().setVisibility(false);
            }
            inputTypeView.setImageResource(R.drawable.tap);
            controls = new TouchControls(this, xserver);
        }
        touchView.setOnTouchListener(controls);
    }

    public void toggleTopBar() {
        if (topPanel.getVisibility() == View.VISIBLE) {
            topPanel.setVisibility(View.INVISIBLE);
        } else {
            topPanel.setVisibility(View.VISIBLE);
        }
    }
}
