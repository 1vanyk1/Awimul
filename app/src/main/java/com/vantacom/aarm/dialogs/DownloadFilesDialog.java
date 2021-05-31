package com.vantacom.aarm.dialogs;

import android.app.AlertDialog;
import android.app.Dialog;
import android.os.Bundle;
import android.os.Environment;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.fragment.app.DialogFragment;

import com.vantacom.aarm.MainActivity;
import com.vantacom.aarm.R;
import com.vantacom.aarm.managers.AppDBManager;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URL;
import java.net.URLConnection;

import static com.vantacom.aarm.managers.FileManager.EXP_PATH;
import static com.vantacom.aarm.managers.FileManager.MAIN_VERSION;

public class DownloadFilesDialog extends DialogFragment {
    private MainActivity activity;
    private Dialog dialog;
    private ProgressBar progressBar;
    private TextView progressText;
    private String status = null;
    private long lengthOfFile = -1;
    private long totalLength = 0;
    private String progress;

    public DownloadFilesDialog(MainActivity activity) {
        this.activity = activity;
    }

    public DownloadFilesDialog() {
        this.activity = (MainActivity) getActivity();
    }

    public void init(MainActivity context) {
        this.activity = context;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        setRetainInstance(true);
        if (savedInstanceState != null) {
            status = savedInstanceState.getString("status", null);
            lengthOfFile = savedInstanceState.getLong("lengthOfFile", -1);
            totalLength = savedInstanceState.getLong("totalLength", 0);
        }
        progress = getString(R.string.downLText);
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View view = inflater.inflate(R.layout.fragment_downloading_files, null);
        progressBar = view.findViewById(R.id.progressBar);
        progressText = view.findViewById(R.id.progressText);
        if (lengthOfFile > 0) {
            progressBar.setProgress((int) ((totalLength * 100) / lengthOfFile));
            updateText();
        }
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
        if (lengthOfFile > 0) {
            savedInstanceState.putLong("lengthOfFile", lengthOfFile);
            savedInstanceState.putLong("totalLength", totalLength);
        }
        dialog.dismiss();
    }

    @Override
    public void onResume() {
        super.onResume();
        dialog.show();
        dialog.setCancelable(false);
    }

    private String convertFileLength(float fileLength) {
        if (fileLength < 4096) {
            return "" + fileLength + " B";
        }
        fileLength /= 1024.0f;
        if (fileLength < 4096) {
            return "" + Math.round(fileLength * 10) / 10.0f + " KB";
        }
        fileLength /= 1024.0f;
        if (fileLength < 4096) {
            return "" + Math.round(fileLength * 10) / 10.0f + " MB";
        }
        fileLength /= 1024.0f;
        return "" + Math.round(fileLength * 10) / 10.0f + " GB";
    }

    private void updateText() {
        progressText.setText(progress + "\n (" + convertFileLength(totalLength) + " / " + convertFileLength(lengthOfFile) + ")");
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
                if (activity != null) {
                    status = "r";
                    String packageName = activity.getPackageName();
                    File root = Environment.getExternalStorageDirectory();
                    AppDBManager sqLiteManager = AppDBManager.getInstance(activity);
                    int count;
                    try {
                        URL url = new URL("https://polar-everglades-72569.herokuapp.com/file");
                        URLConnection connection = url.openConnection();
                        connection.connect();
                        lengthOfFile = connection.getContentLength();
                        totalLength = 0;
                        InputStream input = new BufferedInputStream(url.openStream(),8192);
                        File obbDir = new File(root.toString() + EXP_PATH + packageName);
                        obbDir.mkdir();
                        sqLiteManager.setBool("downloading", true);
                        File obb = new File(obbDir,"main." + MAIN_VERSION + "." + packageName + ".obb");
                        obb.createNewFile();
                        OutputStream output = new FileOutputStream(obb.toString());
                        byte data[] = new byte[1024];
                        while ((count = input.read(data)) != -1) {
                            totalLength += count;
                            if (isAdded()) {
                                getActivity().runOnUiThread(new Runnable() {
                                    @Override
                                    public void run() {
                                        updateText();
                                    }
                                });
                            }
                            progressBar.setProgress((int) ((totalLength * 100) / lengthOfFile));
                            output.write(data, 0, count);
                        }
                        output.flush();
                        output.close();
                        input.close();
                    } catch (Exception e) {
                        e.printStackTrace();
                        status = "e";
                    } finally {
                        if (status.equals("e")) {
                            status = null;
                            dialog.dismiss();
                            dismiss();
                        } else {
                            status = null;
                            dialog.dismiss();
                            dismiss();
                            sqLiteManager.setBool("downloading", false);
                            activity.loadObb();
                        }
                    }
                } else {
                    dialog.dismiss();
                    dismiss();
                }
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
