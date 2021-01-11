package com.vantacom.aarm.wine;

import android.app.Activity;
import android.app.ProgressDialog;

import com.vantacom.aarm.R;

public class LoadingWineDialog {
    private ProgressDialog dialog;

    public LoadingWineDialog(Activity activity) {
        dialog = new ProgressDialog(activity);
        dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        dialog.setCancelable(false);
        dialog.setMessage(activity.getResources().getText(R.string.wineLText));
        dialog.setTitle(R.string.wineLTitle);
    }

    public void show() {
        dialog.show();
    }

    public boolean isShowing() {
        return dialog.isShowing();
    }

    public void dismiss() {
        dialog.hide();
        dialog.dismiss();
        dialog = null;
    }
}
