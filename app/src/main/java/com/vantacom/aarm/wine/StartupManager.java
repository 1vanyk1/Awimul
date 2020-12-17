package com.vantacom.aarm.wine;

import android.content.pm.ApplicationInfo;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Locale;

public class StartupManager {
    private StartupManager() {}

    private static void appendWineSetting(ArrayList<String> wineSettings, String keyName, String argName) {
        wineSettings.add(keyName);
        wineSettings.add(argName);
    }

    public static String[] getWineSetting(File binDir, File libraryDir, File winePrefix, ApplicationInfo applicationInfo) {
        ArrayList<String> wineSettings = new ArrayList<String>();
        File wineDir = new File(binDir, "wine");
        appendWineSetting(wineSettings, "WINELOADER", wineDir.toString());
        appendWineSetting(wineSettings, "WINEPREFIX", winePrefix.toString());
        File wineDLL = new File(libraryDir, "wine");
        appendWineSetting(wineSettings, "WINEDLLPATH", wineDLL.toString());
        appendWineSetting(wineSettings, "LD_LIBRARY_PATH", libraryDir.toString() + ":" + applicationInfo.nativeLibraryDir);
        String lang = Locale.getDefault().getLanguage() + "_" + Locale.getDefault().getCountry() + ".UTF-8";
        appendWineSetting(wineSettings, "LC_ALL", lang);
        appendWineSetting(wineSettings, "LANG", lang);
        appendWineSetting(wineSettings, "PATH", binDir.toString() + ":" + System.getenv("PATH"));
        File logfile = new File(Environment.getExternalStorageDirectory(), "logwine.txt");
        try {
            PrintWriter writer = new PrintWriter(logfile);
            writer.print("");
            writer.close();
        } catch (FileNotFoundException e) {}
        appendWineSetting(wineSettings, "WINEDEBUGLOG", logfile.toString());
        Log.i("WA", "logging to " + logfile.toString());
        return wineSettings.toArray(new String[0]);
    }

    private static String getDriveCFile(String filename) {
        switch (filename) {
            default: return filename;
        }
    }

    private static String getExeFile(String filename) {
        switch (filename) {
            default: return filename;
            case "cmd": return "wineconsole";
        }
    }

    public static String getFilePath(File winePrefix, String filename) {
        String file = getDriveCFile(filename);
        if (new File(winePrefix, "drive_c/winestart." + file).exists()) {
            return "c:\\winestart." + file;
        } else {
            return getExeFile(filename) + ".exe";
        }
    }
}
