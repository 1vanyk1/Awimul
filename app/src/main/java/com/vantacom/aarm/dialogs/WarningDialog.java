package com.vantacom.aarm.dialogs;

import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;

import androidx.fragment.app.DialogFragment;

import com.vantacom.aarm.MainActivity;
import com.vantacom.aarm.R;

public class WarningDialog extends DialogFragment {
    private MainActivity activity;
    private int warning;

    public WarningDialog(MainActivity activity, int warning) {
        this.activity = activity;
        this.warning = warning;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        setRetainInstance(true);
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setTitle(R.string.warning);
        builder.setMessage(warning);
        builder.setCancelable(false);
        builder.setPositiveButton(R.string.ok, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                dialog.cancel();
                activity.finishAndRemoveTask();
            }
        });
        return builder.create();
    }
}
