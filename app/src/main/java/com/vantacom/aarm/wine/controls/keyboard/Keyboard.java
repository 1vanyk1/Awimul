package com.vantacom.aarm.wine.controls.keyboard;

import android.app.Activity;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;

import com.vantacom.aarm.wine.xserver.XServerManager;

public class Keyboard {
    private InputMethodManager keyboard;
    private boolean isShown = false;
    private View view;
    private int hwnd = 0;
    private XServerManager xserver;

    public Keyboard(Activity activity, XServerManager xserver) {
        keyboard = (InputMethodManager)activity.getSystemService(Activity.INPUT_METHOD_SERVICE);
        view = activity.getWindow().getDecorView().getRootView();
        this.xserver = xserver;
    }

    public void toggleKeyboard() {
        if (isShown) {
            hideKeyboard();
        } else {
            showKeyboard();
        }
    }

    public void destroy() {
        hideKeyboard();
        keyboard = null;
        view = null;
        xserver = null;
    }

    public void setHWND(int hwnd) { this.hwnd = hwnd; }

    public void showKeyboard() {
        isShown = true;
        keyboard.showSoftInput(view, InputMethodManager.SHOW_FORCED);
    }

    public void hideKeyboard() {
        isShown = false;
        keyboard.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }

    public boolean pressKey(KeyEvent event) {
        return pressKey(event.getAction(), event.getKeyCode(), event.getMetaState());
    }

    public boolean pressKey(int action, int key, int metaState) {
        if (!xserver.isSystemPaused()) {
            return (boolean) xserver.getWineActivity().invoke("wine_keyboard_event", hwnd, action, key, metaState);
        }
        return false;
    }
}
