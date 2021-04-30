package com.vantacom.aarm.dialogs;

import android.app.AlertDialog;
import android.app.Dialog;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

import androidx.fragment.app.DialogFragment;

import com.vantacom.aarm.R;
import com.vantacom.aarm.UserActivity;
import com.vantacom.aarm.managers.PackageDBManager;

public class RenamePackageDialog extends DialogFragment implements View.OnClickListener {
    private UserActivity activity;
    private EditText renameText;
    private Dialog dialog;
    private TextView ok, cancel;
    private String packageName;
    PackageDBManager packageManager;

    public RenamePackageDialog(UserActivity activity, String packageName, PackageDBManager packageManager) {
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
        renameText.setText(packageName);
        ok = view.findViewById(R.id.ok);
        ok.setOnClickListener(this);
        cancel = view.findViewById(R.id.cancel);
        cancel.setOnClickListener(this);
        // Остальной код
        dialog = builder.create();
        return dialog;

//        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
//        builder.setTitle(R.string.warning);
//        builder.setMessage(R.string.obbWarning);
//        builder.setCancelable(false);
//        builder.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
//            public void onClick(DialogInterface dialog, int id) {
//                dialog.cancel();
//                activity.finishAndRemoveTask();
//            }
//        });
//        return builder.create();
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.cancel) {
            dialog.cancel();
        } else if (v.getId() == R.id.ok) {
            String res = renameText.getText().toString().trim();
            if (res.equals(packageName) || res.equals("")) {
                dialog.cancel();
            }
            if (packageManager.getString(res, "name").equals("")) {
                dialog.cancel();
            }
        }
    }
}