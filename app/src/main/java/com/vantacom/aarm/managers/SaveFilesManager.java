package com.vantacom.aarm.managers;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

public class SaveFilesManager {
    private boolean isFirstLoad;
    private String file;

    public SaveFilesManager(Context context, String name) {
        file = context.getFilesDir() + String.format("/sf/");
        File saveFile = new File(file);
        saveFile.mkdirs();
        file = context.getFilesDir() + String.format("/sf/%s.txt", name);
        loadData();
    }

    public void loadData() {
        isFirstLoad = loadChars(0, true);
    }



    public byte[] loadChars(int i, int len) {
        File saveFile = new File(file);
        try {
            saveFile.createNewFile();
        } catch (Exception e) {
            Log.e("loadChars", e.toString());
        }
        byte[] bytes = new byte[len];
        try {
            FileInputStream is = new FileInputStream(saveFile);
            is.read(bytes, i, len);
            is.close();
            return bytes;
        } catch (Exception e) {
            return null;
        }
    }

    public boolean loadChars(int i, boolean b) {
        byte[] bytes = loadChars(i, 1);
        if (bytes == null) {
            return b;
        } if (bytes[0] == 't') {
            return true;
        } if (bytes[0] == 'f') {
            return false;
        }
        return b;
    }



    public void changeChars(byte[] c, int i) {
        File saveFile = new File(file);
        saveFile.mkdir();
        try {
            FileOutputStream os = new FileOutputStream(saveFile);
            os.write(c, i, c.length);
            os.close();
        } catch (Exception e) {
            Log.e("changeChars", e.toString());
        }
    }

    public void changeChars(boolean b, int i) {
        byte[] bytes = new byte[1];
        if (b) {
            bytes[0] = 't';
        } else {
            bytes[0] = 'f';
        }
        changeChars(bytes, i);
    }



    public boolean getIsFirstLoad() {
        return isFirstLoad;
    }

    public void setIsFirstLoad(boolean ifl) {
        isFirstLoad = ifl;
        changeChars(ifl, 0);
    }
}
