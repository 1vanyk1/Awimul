package com.vantacom.aarm.wine.controls;

import android.app.Activity;
import android.view.View;
import android.view.inputmethod.InputMethodManager;

public class Keyboard {
    private InputMethodManager keyboard;
    private boolean isShown = false;
    private View view;
    private int hwnd = 0;

    public Keyboard(Activity activity) {
        keyboard = (InputMethodManager)activity.getSystemService(Activity.INPUT_METHOD_SERVICE);
        view = activity.getWindow().getDecorView().getRootView();
    }

    public void toggleKeyboard() {
        if (isShown) {
            hideKeyboard();
        } else {
            showKeyboard();
        }
    }

    public void setHWND(int hwnd) { this.hwnd = hwnd; }

    public int getHWND() { return hwnd; }

    public void showKeyboard() {
        isShown = true;
        keyboard.showSoftInput(view, InputMethodManager.SHOW_FORCED);
    }

    public void hideKeyboard() {
        isShown = false;
        keyboard.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }
}
