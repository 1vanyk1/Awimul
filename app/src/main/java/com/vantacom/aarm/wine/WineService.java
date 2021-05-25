package com.vantacom.aarm.wine;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ActivityManager;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.app.TaskStackBuilder;
import android.content.Context;
import android.content.Intent;
import android.graphics.Rect;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.util.Log;
import android.view.KeyEvent;

import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import com.vantacom.aarm.LibraryManager;
import com.vantacom.aarm.MainActivity;
import com.vantacom.aarm.R;
import com.vantacom.aarm.UserActivity;
import com.vantacom.aarm.managers.PackageDBManager;
import com.vantacom.aarm.wine.xserver.XServerManager;

import org.winehq.wine.WineIStream;

import java.io.File;

public class WineService extends Service implements WineIStream {
    public static final int NOTIFY_ID = 1239132112;
    public static String CHANNEL_ID = "Awimul_Notify1239132112";
    private ServiceHandler serviceHandler;
    private Messenger messenger;

    private XServerManager xserver;
    private PackageDBManager sqLiteManager;
    private WineActivity activity;
    private int screenWidth, screenHeight, desktopWidth, desktopHeight;
    private LibraryManager wineActivity;
    private String wineABI, packageName;

    private boolean wineLoaded = false;

    public final static int SET_SCREEN_SIZE = 101;
    public final static int SET_WINE_ABI = 102;
    public final static int SET_ACTIVITY = 111;
    public final static int SET_PACKAGE_NAME = 112;
    public final static int GET_XSERVER = 201;
    public final static int UPDATE_ACTIVITY = 300;
    public final static int START_WINE = 400;
    public final static int TOGGLE_KEYBOARD = 500;
    public final static int SHOW_KEYBOARD = 501;
    public final static int HIDE_KEYBOARD = 502;
    public final static int PRESS_KEY = 503;
    public final static int CHANGE_INPUT_TYPE = 600;

    @Override
    public void createDesktopWindow(int hwnd) {
        xserver.createDesktopWindow(hwnd);
    }

    @Override
    public void onWineLoad() {
        wineLoaded = true;
        activity.onWineLoad();
    }

    @Override
    public void createWindow(int hwnd, boolean isClient, int parent, float scale) {
        xserver.createWindow(hwnd, isClient, parent, scale);
    }

    @Override
    public void windowPosChanged(int hwnd, int vis, int next_hwnd, int owner, int style, Rect win_rect, Rect client_rect, Rect visible_rect) {
        xserver.windowPosChanged(hwnd, vis, next_hwnd, owner, style, win_rect, client_rect, visible_rect);
    }

    @Override
    public void destroyWindow(int hwnd) {
        xserver.destroyWindow(hwnd);
    }

    @Override
    public void setWindowParent(int hwnd, int hwnd_parent, float scale) {
        xserver.setWindowParent(hwnd, hwnd_parent, scale);
    }

    private final class ServiceHandler extends Handler {
        public ServiceHandler() {}

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                default:
                    break;
                case SET_SCREEN_SIZE:
                    int[] size = (int[]) msg.obj;
                    screenWidth = size[0];
                    screenHeight = size[1];
                    desktopWidth = size[2];
                    desktopHeight = size[3];
                    break;
                case SET_ACTIVITY:
                    activity = (WineActivity) msg.obj;
                    break;
                case SET_WINE_ABI:
                    wineABI = (String) msg.obj;
                    break;
                case SET_PACKAGE_NAME:
                    packageName = (String) msg.obj;
                    break;
                case GET_XSERVER:
                    if (msg.replyTo != null) {
                        try {
                            Message message = Message.obtain(null, WineService.GET_XSERVER, 1, 1);
                            message.obj = xserver;
                            msg.replyTo.send(message);
                        } catch (RemoteException e) {
                            e.printStackTrace();
                        }
                    }
                    break;
                case UPDATE_ACTIVITY:
                    activity = (WineActivity) msg.obj;
                    xserver.updateActivity(activity, wineLoaded);
                    if (msg.replyTo != null) {
                        try {
                            Message message = Message.obtain(null, WineService.UPDATE_ACTIVITY, 1, 1);
                            Bundle bundle = new Bundle();
                            bundle.putString("package", packageName);
                            bundle.putBoolean("wineLoaded", wineLoaded);
                            message.obj = bundle;
                            msg.replyTo.send(message);
                        } catch (RemoteException e) {
                            e.printStackTrace();
                        }
                    }
                    break;
                case START_WINE:
                    startWine();
                    break;
                case TOGGLE_KEYBOARD:
                    xserver.getKeyboard().toggleKeyboard();
                    break;
                case SHOW_KEYBOARD:
                    xserver.getKeyboard().showKeyboard();
                    break;
                case HIDE_KEYBOARD:
                    xserver.getKeyboard().hideKeyboard();
                    break;
                case PRESS_KEY:
                    if (msg.obj.getClass().equals(KeyEvent.class)) {
                        xserver.getKeyboard().pressKey((KeyEvent) msg.obj);
                    } else {
                        int[] obj = (int[]) msg.obj;
                        int action = obj[0];
                        int key = obj[1];
                        int metaState = obj[1];
                        xserver.getKeyboard().pressKey(action, key, metaState);
                    }
                    break;
                case CHANGE_INPUT_TYPE:
                    xserver.getCursor().setVisibility((Boolean)msg.obj);
                    if (msg.replyTo != null) {
                        try {
                            Message message = Message.obtain(null, WineService.CHANGE_INPUT_TYPE, 1, 1);
                            message.obj = xserver;
                            msg.replyTo.send(message);
                        } catch (RemoteException e) {
                            e.printStackTrace();
                        }
                    }
                    break;
            }
        }
    }

    private void startWine() {
        startForeground(NOTIFY_ID, Notify.generate(activity, packageName));
        try {
            wineActivity = new LibraryManager("org.winehq.wine.WineActivity");
            wineActivity.invoke("init", this, new File(getFilesDir(), wineABI + "/lib"));
        } catch (Exception e) {
            Log.e("WS", e.toString());
        }
        xserver = new XServerManager(screenWidth, screenHeight, desktopWidth, desktopHeight, activity, wineActivity);
        new Thread(new Runnable() {
            public void run() {
                loadWine(null, getFilesDir());
            }
        }).start();
    }

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    private void loadWine(String path2file, File filesDir) {
        File binDir = new File(filesDir, wineABI + "/bin");
        File libraryDir = new File(filesDir, wineABI + "/lib");
        File winePrefix = new File(filesDir, "prefixes/" + sqLiteManager.getString(packageName, "prefix"));
        String[] wineSettings = StartupManager.getWineSetting(binDir, libraryDir, winePrefix, getApplicationInfo());
        if (path2file == null) {
            path2file = StartupManager.getFilePath(winePrefix, "explorer");
        }
        winePrefix.mkdirs();
        for (int i = 0; i < wineSettings.length; i += 2) {
            Log.i("WS/wineSettings", String.format("%s: %s", wineSettings[i], wineSettings[i + 1]));
        }
        wineActivity.invoke("wine_init", new String[]{wineSettings[1], "explorer", "/desktop=shell,,android", path2file}, wineSettings);
    }

    @Override
    public void onCreate() {
        super.onCreate();
        HandlerThread thread = new HandlerThread("ServiceStartArguments", android.os.Process.THREAD_PRIORITY_BACKGROUND);
        thread.start();
        sqLiteManager = PackageDBManager.getInstance(this);
        serviceHandler = new ServiceHandler();
        messenger = new Messenger(serviceHandler);
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        super.onStartCommand(intent, flags, startId);
        return START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return messenger.getBinder();
    }

    @Override
    public boolean onUnbind(Intent intent) {
        return super.onUnbind(intent);
    }

    @Override
    public void onDestroy() {
        stopForeground(true);
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);
        xserver.destroy();
        notificationManager.cancel(NOTIFY_ID);
    }

    private static class Notify {
        public static Notification generate(Context context, String packageName) {
            NotificationCompat.Builder builder =
                    new NotificationCompat.Builder(context, CHANNEL_ID)
                            .setSmallIcon(R.drawable.ic_launcher_foreground)
                            .setContentTitle(context.getString(R.string.app_name))
                            .setContentText(context.getString(R.string.is_working))
                            .setAutoCancel(true)
                            .setPriority(NotificationCompat.PRIORITY_HIGH)
                            .setCategory(NotificationCompat.CATEGORY_MESSAGE)
                            .setVibrate(null)
                            .setSound(null);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                CharSequence name = context.getString(R.string.app_name);
                String description = context.getString(R.string.is_working);
                int importance = NotificationManager.IMPORTANCE_DEFAULT;
                NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
                channel.setDescription(description);
                NotificationManager notificationManager = context.getSystemService(NotificationManager.class);
                notificationManager.createNotificationChannel(channel);
            }
            Intent notifyIntent = new Intent(context, WineActivity.class);
            notifyIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
            Intent mainActivity = new Intent(context, MainActivity.class);
            Intent userActivity = new Intent(context, UserActivity.class);
            userActivity.putExtra("package", packageName);
            TaskStackBuilder stackBuilder = TaskStackBuilder.create(context);
            stackBuilder.addNextIntent(mainActivity);
            stackBuilder.addNextIntent(userActivity);
            stackBuilder.addNextIntent(notifyIntent);
            PendingIntent notifyPendingIntent = stackBuilder.getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT);
            NotificationManagerCompat notificationManager = NotificationManagerCompat.from(context);
            builder.setContentIntent(notifyPendingIntent);
            Notification notification = builder.build();
            notification.flags = notification.flags | Notification.FLAG_ONGOING_EVENT;
            return notification;
        }
    }

    public static boolean isServiceRunning(Activity activity) {
        ActivityManager manager = (ActivityManager) activity.getSystemService(Context.ACTIVITY_SERVICE);
        for (ActivityManager.RunningServiceInfo service : manager.getRunningServices(Integer.MAX_VALUE)) {
            if (WineService.class.getName().equals(service.service.getClassName())) {
                return true;
            }
        }
        return false;
    }
}
