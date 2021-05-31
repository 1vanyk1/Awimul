package com.vantacom.aarm.dialogs;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;

import androidx.fragment.app.DialogFragment;

import com.vantacom.aarm.R;
import com.vantacom.aarm.managers.AppDBManager;
import com.vantacom.aarm.managers.FileManager;

public class LoadingFilesDialog extends DialogFragment {
    private Context context;
    private Dialog dialog;
    private String status = null;

    public LoadingFilesDialog(Context context) {
        this.context = context;
    }

    public LoadingFilesDialog() {
        this.context = getContext();
    }

    public void init(Context context) {
        this.context = context;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        setRetainInstance(true);
        if (savedInstanceState != null) {
            status = savedInstanceState.getString("status", null);
        }
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View view = inflater.inflate(R.layout.fragment_loading_files, null);
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
        dialog.dismiss();
    }

    @Override
    public void onResume() {
        super.onResume();
        dialog.show();
        dialog.setCancelable(false);
    }

    @Override
    public void onStart() {
        super.onStart();
        dialog.show();
        dialog.setCancelable(false);
        if (status != null && status.equals("r")) {
            return;
        }
        new Thread(new Runnable() {
            @Override
            public void run() {
                if (context != null) {
                    status = "r";
                    FileManager.copyAPKExpansionFiles(context, context.getFilesDir() + "/");
                    AppDBManager sqLiteManager = AppDBManager.getInstance(context);
                    sqLiteManager.setBool("firstLoad", false);
                    status = null;
                }
                dialog.dismiss();
                dismiss();
            }
        }).start();
    }

    @Override
    public void onDestroyView() {
        Dialog dialog = getDialog();
        if (dialog != null && getRetainInstance()) {
            dialog.setDismissMessage(null);
        }
        super.onDestroyView();
    }
}
