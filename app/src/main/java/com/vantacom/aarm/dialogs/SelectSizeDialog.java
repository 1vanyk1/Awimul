package com.vantacom.aarm.dialogs;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.util.DisplayMetrics;

import androidx.fragment.app.DialogFragment;

import com.vantacom.aarm.R;
import com.vantacom.aarm.UserActivity;
import com.vantacom.aarm.managers.PackageDBManager;

public class SelectSizeDialog extends DialogFragment {
    private UserActivity activity;
    private PackageDBManager packageManager;
    private String packageName;
    private final String[] sizes = {"native",   "320x240", "480x320", "640x480", "640x360", "720x480", "768x576", "800x600", "1024x768", "1280x720", "1280x800", "1280x960", "1280x1024", "1440x900", "1600x900", "1600x1200", "1680x1050", "1920x1080"};
    private final String[] proportions = {null, "4:3",     "3:2",     "4:3",     "16:9",     "3:2",     "4:3",     "4:3",     "4:3",      "16:9",     "16:10",    "4:3",      "5:4",       "16:10",    "16:9",     "4:3",       "16:10",     "16:9"};

    public SelectSizeDialog(UserActivity activity, PackageDBManager packageManager, String packageName) {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        int screenWidth, screenHeight;
        if (displayMetrics.widthPixels < displayMetrics.heightPixels) {
            screenWidth = displayMetrics.heightPixels;
            screenHeight = displayMetrics.widthPixels;
        } else {
            screenWidth = displayMetrics.widthPixels;
            screenHeight = displayMetrics.heightPixels;
        }
        sizes[0] = String.format("%s (%dx%d)", sizes[0], screenWidth, screenHeight);
        this.activity = activity;
        this.packageManager = packageManager;
        this.packageName = packageName;
    }

    public SelectSizeDialog() {}

    public void init(UserActivity activity, PackageDBManager packageManager, String packageName) {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        activity.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        int screenWidth = displayMetrics.widthPixels;
        int screenHeight = displayMetrics.heightPixels;
        sizes[0] = String.format("%s (%dx%d)", sizes[0], screenWidth, screenHeight);
        this.activity = activity;
        this.packageManager = packageManager;
        this.packageName = packageName;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setTitle(R.string.setSize);
        builder.setCancelable(false);
        String[] array = new String[sizes.length];
        for (int i = 0; i < array.length; i++) {
            if (sizes[i].equals("native")) {
                array[i] = "Screen size";
            } else {
                try {
                    String proportion = proportions[i];
                    if (proportion == null) {
                        array[i] = sizes[i];
                    } else {
                        array[i] = String.format("%s (%s)", sizes[i], proportion);
                    }
                } catch (Exception e) {
                    array[i] = sizes[i];
                }
            }
        }
        builder.setItems(array, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int item) {
                String size;
                if (item == 0) {
                    size = "native";
                } else {
                    size = sizes[item];
                }
                packageManager.setString(packageName, "size", size);
                activity.updateChangeSizeText(size);
                dialog.cancel();
            }
        });
        return builder.create();
    }
}
