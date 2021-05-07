package com.vantacom.aarm;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.Build;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

public class BackgroundService extends Service {
    public static final int NOTIFY_ID = 1239132112;
    public static String CHANNEL_ID = "Awimul_Notify1239132112";
    private ServiceHandler serviceHandler;

//    public class LocalBinder extends Binder {
//        public BackgroundService getService() {
//            return BackgroundService.this;
//        }
//    }

    private final class ServiceHandler extends Handler {
        public ServiceHandler() {}

        @Override
        public void handleMessage(Message msg) {
//            Log.e("service", msg.toString());
            switch (msg.arg1) {
                default:
                    break;
                case 1:
                    try {
                        Thread.sleep(500);
//                        Log.e("service", "5000");
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                    break;
                case 2:
                    stopSelf(msg.arg1);
                    break;
            }
        }
    }

    @Override
    public void onCreate() {
        super.onCreate();
//        Log.e("service", "create");
        HandlerThread thread = new HandlerThread("ServiceStartArguments", android.os.Process.THREAD_PRIORITY_BACKGROUND);
        thread.start();
        serviceHandler = new ServiceHandler();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        super.onStartCommand(intent, flags, startId);
//        Log.e("service", "service starting");
        startForeground(NOTIFY_ID, Notify.generate(this));
        Message msg = serviceHandler.obtainMessage();
        msg.arg1 = 1;
        serviceHandler.sendMessage(msg);
        return START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        Toast.makeText(this, "bind", Toast.LENGTH_LONG).show();
//        Log.e("service", "service bind");
        return null;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        Toast.makeText(this, "unbind", Toast.LENGTH_LONG).show();
//        Log.e("service", "service unbind");
        return super.onUnbind(intent);
    }

    @Override
    public void onDestroy() {
        stopForeground(true);
        Toast.makeText(this, "done", Toast.LENGTH_LONG).show();
//        Log.e("service", "service done");
        NotificationManagerCompat notificationManager = NotificationManagerCompat.from(this);
        notificationManager.cancel(NOTIFY_ID);
    }

    private static class Notify {
        public static Notification generate(Context context) {
            NotificationCompat.Builder builder =
                    new NotificationCompat.Builder(context, CHANNEL_ID)
                            .setSmallIcon(R.drawable.arror)
                            .setContentTitle(context.getString(R.string.app_name))
                            .setContentText(context.getString(R.string.is_working))
                            .setAutoCancel(true)
                            .setPriority(NotificationCompat.PRIORITY_HIGH)
                            .setCategory(NotificationCompat.CATEGORY_MESSAGE);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                CharSequence name = context.getString(R.string.app_name);
                String description = context.getString(R.string.is_working);
                int importance = NotificationManager.IMPORTANCE_DEFAULT;
                NotificationChannel channel = new NotificationChannel(CHANNEL_ID, name, importance);
                channel.setDescription(description);
                NotificationManager notificationManager = context.getSystemService(NotificationManager.class);
                notificationManager.createNotificationChannel(channel);
            }
            NotificationManagerCompat notificationManager = NotificationManagerCompat.from(context);
            Notification notification = builder.build();
            notification.flags = notification.flags | Notification.FLAG_ONGOING_EVENT;
            return notification;
        }
    }
}
