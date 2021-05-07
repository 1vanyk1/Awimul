package com.vantacom.aarm.dialogs;

import android.app.AlertDialog;
import android.app.Dialog;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import androidx.fragment.app.DialogFragment;

import com.vantacom.aarm.R;
import com.vantacom.aarm.UserActivity;
import com.vantacom.aarm.managers.PackageDBManager;

public class RenamePackageDialog extends DialogFragment implements View.OnClickListener, View.OnKeyListener {
    private UserActivity activity;
    private EditText renameText;
    private Dialog dialog;
    private TextView ok, cancel;
    private String packageName;
    PackageDBManager packageManager;

    public RenamePackageDialog(UserActivity activity, PackageDBManager packageManager, String packageName) {
        this.activity = activity;
        this.packageName = packageName;
        this.packageManager = packageManager;
    }

    public RenamePackageDialog() {}

    @Override
    public void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        savedInstanceState.putString("text", renameText.getText().toString().trim());
        dialog.dismiss();
    }

    public void init(UserActivity activity, PackageDBManager packageManager, String packageName) {
        this.activity = activity;
        this.packageName = packageName;
        this.packageManager = packageManager;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View view = inflater.inflate(R.layout.fragment_rename_prefix, null);
        builder.setView(view);
        renameText = view.findViewById(R.id.rename);
        if (savedInstanceState == null) {
            renameText.setText(packageName);
        } else {
            renameText.setText(savedInstanceState.getString("text", packageName));
        }
        renameText.setOnKeyListener(this);
        ok = view.findViewById(R.id.ok);
        ok.setOnClickListener(this);
        cancel = view.findViewById(R.id.cancel);
        cancel.setOnClickListener(this);
        dialog = builder.create();
        return dialog;
    }

    public void rename() {
        String res = renameText.getText().toString().trim();
        if (res.equals(packageName) || res.equals("")) {
            dialog.cancel();
        }
        if (packageManager.getString(res, "name").equals("")) {
            packageManager.setString(packageName, "name", res);
            activity.updateName(res);
            dialog.cancel();
        }
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.cancel) {
            dialog.cancel();
        } else if (v.getId() == R.id.ok) {
            rename();
        }
    }

    @Override
    public boolean onKey(View v, int keyCode, KeyEvent event) {
        if (keyCode == 66) {
            rename();
        }
        return false;
    }
}