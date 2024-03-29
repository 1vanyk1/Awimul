package com.vantacom.aarm.dialogs;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;

import androidx.fragment.app.DialogFragment;

import com.vantacom.aarm.R;
import com.vantacom.aarm.managers.FileManager;

public class DeletingPackageDialog extends DialogFragment {
    private Activity activity;
    private String packageName;
    private Dialog dialog;
    private String status = null;

    public DeletingPackageDialog(Activity activity, String packageName) {
        this.activity = activity;
        this.packageName = packageName;
    }

    public DeletingPackageDialog() {}

    public void init(Activity activity, String packageName) {
        this.activity = activity;
        this.packageName = packageName;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        setRetainInstance(true);
        if (savedInstanceState != null) {
            status = savedInstanceState.getString("status", null);
            packageName = savedInstanceState.getString("packageName", packageName);
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View view = inflater.inflate(R.layout.fragment_deleting_prefix, null);
        builder.setView(view);
        builder.setCancelable(false);
        if (dialog != null) {
            dialog.dismiss();
        }
        dialog = builder.create();
    }

    @Override
    public void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        savedInstanceState.putString("status", status);
        savedInstanceState.putString("packageName", packageName);
        dialog.dismiss();
    }

    @Override
    public void onResume() {
        super.onResume();
        if (status != null && status.equals("r")) {
            dialog.show();
            dialog.setCancelable(false);
        }
    }

    @Override
    public void onStart() {
        super.onStart();
        if (status != null && status.equals("r")) {
            return;
        }
        dialog.show();
        dialog.setCancelable(false);
        new Thread(new Runnable() {
            @Override
            public void run() {
                status = "r";
                FileManager.deleteUser(activity, packageName);
                dialog.dismiss();
                activity.finish();
                status = null;
            }
        }).start();
    }
}