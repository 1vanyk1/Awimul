 package com.vantacom.aarm.wine;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.DisplayMetrics;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.vantacom.aarm.R;
import com.vantacom.aarm.dialogs.ConfirmTurnOff;
import com.vantacom.aarm.dialogs.HelpMenuDialog;
import com.vantacom.aarm.dialogs.LoadingWineDialog;
import com.vantacom.aarm.managers.ConsoleManager;
import com.vantacom.aarm.managers.PackageDBManager;
import com.vantacom.aarm.managers.ProcessManager;
import com.vantacom.aarm.wine.controls.MouseActions;
import com.vantacom.aarm.wine.controls.touchscreen.BaseControls;
import com.vantacom.aarm.wine.controls.touchscreen.MouseControls;
import com.vantacom.aarm.wine.controls.touchscreen.TouchControls;
import com.vantacom.aarm.wine.xserver.XServerManager;


 public class WineActivity extends AppCompatActivity implements View.OnTouchListener {
    private ConstraintLayout view;
    private String wineABI;
    private ProcessManager processManager;
    private PackageDBManager sqLiteManager;
    private XServerManager xserver;
    private BaseControls controls;
    private String packageName;
    private boolean workInBG, showCursor;
    private String inputType;

    private ImageView keyboardView, inputTypeView, helpMenuView, exitView;
    private View touchView, topPanel;

    private boolean ifTurnedOff = false;

    private LoadingWineDialog dialog;

    private Messenger replyMessenger = new Messenger(new HandlerReplyMsg());
    private boolean mShouldUnbind;
    private Messenger msgService;
    private boolean isBound;

    private void hideSystemUI() {
        getWindow().getDecorView().setSystemUiVisibility( View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    private int[] updateScreenInfo() {
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
        return new int[]{screenWidth, screenHeight, desktopWidth, desktopHeight};
    }

    ServiceConnection connection = new ServiceConnection() {
        @Override
        public void onServiceDisconnected(ComponentName name) {
            isBound = false;
        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            isBound = true;
            msgService = new Messenger(service);
        }
    };

    public void sendSimpleMessage(int message_type) {
        try {
            Message message = Message.obtain(null, message_type, 1, 1);
            if (msgService != null) {
                msgService.send(message);
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    public void startWineService(int[] screenInfo) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                int wait = 128;
                while (!isBound && wait > 0) {
                    try {
                        Thread.sleep(128);
                        wait -= 1;
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                if (isBound) {
                    Message message;
                    try {
                        message = Message.obtain(null, WineService.SET_SCREEN_SIZE, 1, 1);
                        message.obj = screenInfo;
                        msgService.send(message);

                        message = Message.obtain(null, WineService.SET_ACTIVITY, 1, 1);
                        message.obj = WineActivity.this;
                        msgService.send(message);

                        message = Message.obtain(null, WineService.SET_WINE_ABI, 1, 1);
                        message.obj = wineABI;
                        msgService.send(message);

                        message = Message.obtain(null, WineService.SET_PACKAGE_NAME, 1, 1);
                        message.obj = packageName;
                        msgService.send(message);

                        message = Message.obtain(null, WineService.START_WINE, 1, 1);
                        msgService.send(message);

                        message = Message.obtain(null, WineService.GET_XSERVER, 1, 1);
                        message.replyTo = replyMessenger;
                        msgService.send(message);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }

    public void connectWineService() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                int wait = 128;
                while (!isBound && wait > 0) {
                    try {
                        Thread.sleep(64);
                        wait -= 1;
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                if (isBound) {
                    Message message;
                    try {
                        message = Message.obtain(null, WineService.GET_XSERVER, 1, 1);
                        message.replyTo = replyMessenger;
                        msgService.send(message);

                        message = Message.obtain(null, WineService.UPDATE_ACTIVITY, 1, 1);
                        message.replyTo = replyMessenger;
                        message.obj = WineActivity.this;
                        msgService.send(message);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }

    public class HandlerReplyMsg extends Handler {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case WineService.GET_XSERVER:
                    WineActivity.this.xserver = (XServerManager) msg.obj;
                    break;
                case WineService.UPDATE_ACTIVITY:
                    Bundle bundle = (Bundle) msg.obj;
                    packageName = bundle.getString("package");
                    boolean wineLoaded = bundle.getBoolean("wineLoaded", false);
                    workInBG = sqLiteManager.isBool(packageName, "workInBackground");
                    showCursor = sqLiteManager.isBool(packageName, "showCursor");
                    updateScreenInfo();
                    if (!wineLoaded) {
                        dialog = new LoadingWineDialog();
                        FragmentManager manager = getSupportFragmentManager();
                        FragmentTransaction transaction = manager.beginTransaction();
                        dialog.show(transaction, "dialog");
                    } else {
                        processManager = new ProcessManager();
                        processManager.pauseSystem();
                        processManager.resumeSystem();
                        changeInputType("touch");
                        runOnUiThread(new Runnable() {
                            public void run() {
                                topPanel.setVisibility(View.VISIBLE);
                                view.addView(xserver.getDesktopView());
                            }
                        });
                    }
                    hideSystemUI();
                    break;
                default:
                    break;
            }
        }
    }

    void doBindService() {
        if (bindService(new Intent(WineActivity.this, WineService.class), connection, Context.BIND_AUTO_CREATE)) {
            mShouldUnbind = true;
        }
    }

    void doUnbindService() {
        if (mShouldUnbind) {
            unbindService(connection);
            mShouldUnbind = false;
        }
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
        if (xserver != null) {
            connectWineService();
        }
        hideSystemUI();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.requestWindowFeature(Window.FEATURE_NO_TITLE);
        this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        this.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        try {this.getSupportActionBar().hide();}
        catch (NullPointerException e) {}
        setContentView(R.layout.activity_wine);
        view = findViewById(R.id.mainLayout);
        touchView = findViewById(R.id.touchView);
        topPanel = findViewById(R.id.topPanel);
        keyboardView = findViewById(R.id.keyboard);
        keyboardView.setOnTouchListener(this);
        inputTypeView = findViewById(R.id.input_type);
        inputTypeView.setOnTouchListener(this);
        helpMenuView = findViewById(R.id.help_menu);
        helpMenuView.setOnTouchListener(this);
        exitView = findViewById(R.id.exit);
        exitView.setOnTouchListener(this);

        sqLiteManager = PackageDBManager.getInstance(this);
        wineABI = "armeabi-v7a";
        if (WineService.isServiceRunning(this)) {
            doBindService();
            connectWineService();
            return;
        }
        packageName = getIntent().getStringExtra("package");
        boolean wineLoaded = getIntent().getBooleanExtra("wineLoaded", false);
        workInBG = sqLiteManager.isBool(packageName, "workInBackground");
        showCursor = sqLiteManager.isBool(packageName, "showCursor");
        int[] screenInfo = updateScreenInfo();
        if (!wineLoaded) {
            dialog = new LoadingWineDialog();
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialog.show(transaction, "dialog");
        }
        hideSystemUI();
        startService(new Intent(this, WineService.class));
        doBindService();
        startWineService(screenInfo);
    }

     @Override
     public void onSaveInstanceState(Bundle savedInstanceState) {
         super.onSaveInstanceState(savedInstanceState);
         savedInstanceState.putBoolean("mShouldUnbind", mShouldUnbind);
     }

     public void onWineLoad() {
        processManager = new ProcessManager();
        changeInputType("touch");
        runOnUiThread(new Runnable() {
            public void run() {
                dialog.hide();
                topPanel.setVisibility(View.VISIBLE);
            }
        });
        if (ifTurnedOff && !workInBG) {
            processManager.pauseSystem();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (dialog != null) {
            dialog.hide();
        }
        ifTurnedOff = true;
        if (processManager != null && !workInBG) {
            processManager.pauseSystem();
        }
        sendSimpleMessage(WineService.HIDE_KEYBOARD);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (!ifTurnedOff) {
            onTurnOff();
        } else {
            doUnbindService();
            view.removeAllViews();
        }
    }

    public void onTurnOff() {
        doUnbindService();
        Intent service = new Intent(this, WineService.class);
        stopService(service);
        view.removeAllViews();
        ConsoleManager.runCommand("wineserver -k");
        ifTurnedOff = true;
        finish();
    }

    @Override
    public void onBackPressed() {
        hideSystemUI();
    }

    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if (event.getKeyCode() == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {
            if (event.getDeviceId() < 0) {
                xserver.getKeyboard().pressKey(event.getAction(), 111, 0);
            } else if ((InputDevice.getDevice(event.getDeviceId()).getSources() & InputDevice.SOURCE_MOUSE) == InputDevice.SOURCE_MOUSE) {
                if (event.getAction() == KeyEvent.ACTION_UP) {
                    MouseActions.singleRightButtonClick(xserver.getCursor().getX(), xserver.getCursor().getY(), xserver.getWineActivity(), xserver.getFocusedWindow());
                }
            }
        } else if (!isSystemPaused()) {
            xserver.getKeyboard().pressKey(event);
        }
        return super.dispatchKeyEvent(event);
    }

    public void createDesktopWindow(XServerManager xserver) {
        runOnUiThread(new Runnable() {
            public void run() {
                view.addView(xserver.getDesktopView());
            }
        });
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if ((InputDevice.getDevice(event.getDeviceId()).getSources() & InputDevice.SOURCE_TOUCHSCREEN) != InputDevice.SOURCE_TOUCHSCREEN) {
            return false;
        }
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            if (v.getId() == R.id.keyboard) {
                sendSimpleMessage(WineService.TOGGLE_KEYBOARD);
            } else if (v.getId() == R.id.exit) {
                ConfirmTurnOff dialog = new ConfirmTurnOff(this);
                FragmentManager manager = getSupportFragmentManager();
                FragmentTransaction transaction = manager.beginTransaction();
                dialog.show(transaction, "dialog");
            } else if (v.getId() == R.id.help_menu) {
                int menuId = -1;
                if (inputType.equals("touch")) {
                    menuId = HelpMenuDialog.TOUCH_SCREEN_HELP_MENU;
                } else {
                    menuId = HelpMenuDialog.MOUSE_HELP_MENU;
                }
                HelpMenuDialog dialog = new HelpMenuDialog(menuId);
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
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (inputType.equals("mouse")) {
                    xserver.getCursor().setVisibility(true);
                    inputTypeView.setImageResource(R.drawable.mouse);
                    controls = new MouseControls(WineActivity.this, xserver);
                } else {
                    xserver.getCursor().setVisibility(showCursor);
                    inputTypeView.setImageResource(R.drawable.tap);
                    controls = new TouchControls(WineActivity.this, xserver);
                }
                touchView.setOnTouchListener(controls);
                touchView.setOnGenericMotionListener(controls);
            }
        });
    }

    public void toggleTopBar() {
        if (topPanel.getVisibility() == View.VISIBLE) {
            topPanel.setVisibility(View.INVISIBLE);
        } else {
            topPanel.setVisibility(View.VISIBLE);
        }
    }
}
