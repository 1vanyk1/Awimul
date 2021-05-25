package com.vantacom.aarm.dialogs;

import android.app.AlertDialog;
import android.app.Dialog;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import androidx.fragment.app.DialogFragment;
import androidx.recyclerview.widget.RecyclerView;

import com.vantacom.aarm.R;
import com.vantacom.aarm.adapters.HelpMenuAdapter;

import java.util.ArrayList;

public class HelpMenuDialog extends DialogFragment {
    private Dialog dialog;
    private TextView titleView;
    private RecyclerView recyclerView;
    private HelpMenuAdapter adapter;

    public final static int TOUCH_SCREEN_HELP_MENU = 0;
    public final static int MOUSE_HELP_MENU = 1;

    private int menuId;

    private ArrayList<String> actions, descriptions;

    private void addLine(String action, String description) {
        actions.add(action);
        descriptions.add(description);
    }

    private void generateHelpMenu() {
        actions = new ArrayList<String>();
        descriptions = new ArrayList<String>();
        switch (menuId) {
            default:
                this.menuId = -1;
                break;
            case TOUCH_SCREEN_HELP_MENU:
                titleView.setText(R.string.help_menu_title_touchscreen);
                addLine(getString(R.string.left_button), getString(R.string.short_tap));
                addLine(getString(R.string.right_button), getString(R.string.long_tap));
                addLine(getString(R.string.mouse_wheel), getString(R.string.short_tap_and_move));
                addLine(getString(R.string.drag_and_drop), getString(R.string.long_tap_and_move));
                addLine(getString(R.string.zoom), getString(R.string.two_taps_and_move));
                addLine(getString(R.string.toggle_keyboard), getString(R.string.two_taps));
                addLine(getString(R.string.toggle_title_bar), getString(R.string.three_taps));
                break;
            case MOUSE_HELP_MENU:
                titleView.setText(R.string.help_menu_title_mouse);
                addLine(getString(R.string.move_mouse), getString(R.string.move));
                addLine(getString(R.string.left_button), getString(R.string.short_tap));
                addLine(getString(R.string.right_button), getString(R.string.long_tap));
                addLine(getString(R.string.drag_and_drop), getString(R.string.long_tap_and_move));
                addLine(getString(R.string.zoom), getString(R.string.two_taps_and_move));
                addLine(getString(R.string.toggle_keyboard), getString(R.string.two_taps));
                addLine(getString(R.string.toggle_title_bar), getString(R.string.three_taps));
                break;
        }
    }

    public HelpMenuDialog() {
        this.menuId = -1;
    }

    public HelpMenuDialog(int menuId) {
        this.menuId = menuId;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        setRetainInstance(true);
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View view = inflater.inflate(R.layout.fragment_help_menu, null);
        builder.setView(view);
        recyclerView = view.findViewById(R.id.help_lines_list);
        titleView = view.findViewById(R.id.title);
        generateHelpMenu();
        adapter = new HelpMenuAdapter(actions, descriptions);
        recyclerView.setAdapter(adapter);
        builder.setCancelable(true);
        if (dialog != null) {
            dialog.dismiss();
        }
        dialog = builder.create();
        return dialog;
    }

    @Override
    public void onResume() {
        super.onResume();
        if (menuId == -1) {
            if (dialog != null) {
                dialog.dismiss();
            }
            dismiss();
        }
    }
}

