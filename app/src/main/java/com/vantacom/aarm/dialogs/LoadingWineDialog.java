package com.vantacom.aarm.dialogs;

import android.app.AlertDialog;
import android.app.Dialog;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;

import androidx.fragment.app.DialogFragment;

import com.vantacom.aarm.R;

public class LoadingWineDialog extends DialogFragment {
    private Dialog dialog;

    public LoadingWineDialog() {}

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        setRetainInstance(true);
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View view = inflater.inflate(R.layout.fragment_loading_wine, null);
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
        dialog.dismiss();
    }

    @Override
    public void onResume() {
        super.onResume();
        dialog.show();
        dialog.setCancelable(false);
    }

    @Override
    public void onDestroyView() {
        if (dialog != null) {
            dialog.dismiss();
        }
        Dialog dialog = getDialog();
        if (dialog != null && getRetainInstance()) {
            dialog.setDismissMessage(null);
        }
        super.onDestroyView();
    }

    public void hide() {
        if (dialog != null) {
            dialog.hide();
            try{
                dialog.dismiss();
            } catch (Exception e) {}
            dialog = null;
        }
        dismiss();
    }
}
