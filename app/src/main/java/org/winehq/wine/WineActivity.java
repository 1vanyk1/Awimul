package org.winehq.wine;

import android.app.Activity;
import android.util.Log;

public class WineActivity extends Activity {
    public void runWine(String path2cmd, String[] wineSettings) {
        Log.e("wine", "2");
        Log.e("wine", wine_init(new String[]{wineSettings[1], "explorer.exe", "/desktop=shell,,android", path2cmd}, wineSettings));
    }

    public native String wine_init(String[] paramArrayOfString1, String[] paramArrayOfString2);
}
