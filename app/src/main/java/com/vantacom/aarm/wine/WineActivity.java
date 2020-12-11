package com.vantacom.aarm.wine;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;

import com.vantacom.aarm.R;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.Locale;

public class WineActivity extends Activity {
    private org.winehq.wine.WineActivity wineActivity;
    private File filesDir;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_wine);
        wineActivity = new org.winehq.wine.WineActivity();
        filesDir = getFilesDir();
        new Thread(new Runnable()
        {
            public void run()
            {
                loadWine(null);
            }
        }).start();
    }

    private String[] appendWineSetting(String[] wineSettings, String keyName, String argName) {
        String[] wineSettings_res;
        if (wineSettings == null) {
            wineSettings_res = new String[2];
            wineSettings_res[0] = keyName;
            wineSettings_res[1] = argName;
        } else {
            wineSettings_res = new String[wineSettings.length + 2];
            for (int i = 0; i < wineSettings.length; i++) {
                wineSettings_res[i] = wineSettings[i];
            }
            wineSettings_res[wineSettings.length] = keyName;
            wineSettings_res[wineSettings.length + 1] = argName;
        }
        return wineSettings_res;
    }

    private String[] getWineSetting(File binDir, File libraryDir, File winePrefix) {
        String[] wineSettings;
        File wineDir = new File(binDir, "wine");
        wineSettings = appendWineSetting(null, "WINELOADER", wineDir.toString());
        wineSettings = appendWineSetting(wineSettings, "WINEPREFIX", winePrefix.toString());
        File wineDLL = new File(libraryDir, "wine");
        wineSettings = appendWineSetting(wineSettings, "WINEDLLPATH", wineDLL.toString());
        wineSettings = appendWineSetting(wineSettings, "LD_LIBRARY_PATH", libraryDir.toString() + ":" + getApplicationInfo().nativeLibraryDir);
        String lang = Locale.getDefault().getLanguage() + "_" + Locale.getDefault().getCountry() + ".UTF-8";
        wineSettings = appendWineSetting(wineSettings, "LC_ALL", lang);
        wineSettings = appendWineSetting(wineSettings, "LANG", lang);
        wineSettings = appendWineSetting(wineSettings, "PATH", binDir.toString() + ":" + System.getenv("PATH"));
        File logfile = new File(Environment.getExternalStorageDirectory(), "logwine.txt");
        try {
            PrintWriter writer = new PrintWriter(logfile);
            writer.print("");
            writer.close();
        } catch (FileNotFoundException e) {}
        wineSettings = appendWineSetting(wineSettings, "WINEDEBUGLOG", logfile.toString());
        Log.i("WA", "logging to " + logfile.toString());
        return wineSettings;
    }

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    public void loadWine(String path2cmd) {
        String wineABI = "armeabi-v7a";
        Log.e("canExecute", new File(filesDir, wineABI + "/bin/wineserver").toString());
        if (new File(filesDir, wineABI + "/bin/wineserver").canExecute()) {
            Log.e("canExecute", wineABI + "/bin/wineserver");
        }
        File binDir = new File(filesDir, wineABI + "/bin");
        File libraryDir = new File(filesDir, wineABI + "/lib");
        File winePrefix = new File(filesDir, "prefix");
        String[] wineSettings = getWineSetting(binDir, libraryDir, winePrefix);
        if (path2cmd == null) {
            if (!new File(winePrefix, "drive_c/winestart.cmd").exists()) {
                path2cmd = "wineconsole.exe";
            } else {
                path2cmd = "c:\\winestart.cmd";
            }
        }
        try {
            System.loadLibrary("wine");
        }
        catch (UnsatisfiedLinkError e) {
            Log.e("WA", e.toString());
            System.load(libraryDir.toString() + "/libwine.so");
        }
        winePrefix.mkdirs();
        Log.e("WA", "everything works!");
        for (String wineSetting : wineSettings) {
            Log.i("WA/wineSettings", wineSetting);
        }
        Log.i("WA", path2cmd);
        wineActivity.runWine(path2cmd, wineSettings);
//        wineActivity.wine_init(new String[] { wineSettings[1], "explorer.exe", "/desktop=shell,,android", path2cmd }, wineSettings);
        Log.e("WA", "2");
    }
}
