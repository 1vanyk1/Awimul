package com.vantacom.aarm.dialogs;

import android.app.AlertDialog;
import android.app.Dialog;
import android.os.Bundle;

import androidx.fragment.app.DialogFragment;

import com.vantacom.aarm.R;
import com.vantacom.aarm.UserActivity;
import com.vantacom.aarm.managers.FileManager;
import com.vantacom.aarm.managers.PackageDBManager;

public class DeletingPackageDialog extends DialogFragment {
    private UserActivity activity;
    private String packageName;
    private Dialog dialog;

    public DeletingPackageDialog(UserActivity activity, String packageName) {
        this.activity = activity;
        this.packageName = packageName;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setTitle(R.string.packageDTitle);
        builder.setMessage(R.string.packageDText);
        builder.setCancelable(false);
        dialog = builder.create();
        return dialog;
    }

    @Override
    public void onStart() {
        super.onStart();
        dialog.show();
        dialog.setCancelable(false);
        new Thread(new Runnable() {
            @Override
            public void run() {
                PackageDBManager packageManager = PackageDBManager.getInstance(activity);
                FileManager.deleteFolder(activity, FileManager.getPrefixPath(activity, "prefixes/" + packageManager.getString(packageName, "prefix")));
                packageManager.deletePackage(packageName);
                dialog.dismiss();
                activity.finish();
            }
        }).start();
    }
}