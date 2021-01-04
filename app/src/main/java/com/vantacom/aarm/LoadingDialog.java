package com.vantacom.aarm;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Environment;
import android.util.Log;

import static android.os.Environment.DIRECTORY_DOWNLOADS;

public class LoadingDialog extends AsyncTask<Context, Void, Void> {
    private ProgressDialog dialog;
    private CharSequence text;

    public LoadingDialog(Activity activity) {
        text = activity.getResources().getText(R.string.unpackLText);
        dialog = new ProgressDialog(activity);
        dialog.setCancelable(false);
    }

    @Override
    protected void onPreExecute() {
        dialog.setMessage(text);
        dialog.show();
    }

    @Override
    protected Void doInBackground(Context... args) {
        Log.e("loader", args[0].getFilesDir().toString());
        FileManager.copyAPKExpansionFiles(args[0], 1, 0, args[0].getFilesDir() + "/");
        return null;
    }
    @Override
    protected void onPostExecute(Void result) {
        if (dialog.isShowing()) {
            dialog.dismiss();
        }
    }
}
