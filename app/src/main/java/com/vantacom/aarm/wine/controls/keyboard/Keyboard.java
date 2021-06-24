package com.vantacom.aarm.wine.controls.keyboard;

import android.app.Activity;
import android.graphics.Rect;
import android.os.Build;
import android.util.TypedValue;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver;
import android.view.inputmethod.InputMethodManager;

import com.vantacom.aarm.wine.xserver.XServerManager;

public class Keyboard implements OnKeyboardVisibilityListener {
    private InputMethodManager keyboard;
    private boolean isShown = false;
    private View view;
    private XServerManager xserver;
    private Activity activity;
    private boolean isShiftPressed;

    public Keyboard(Activity activity, XServerManager xserver) {
        keyboard = (InputMethodManager)activity.getSystemService(Activity.INPUT_METHOD_SERVICE);
        view = activity.getWindow().getDecorView().getRootView();
        this.activity = activity;
        this.xserver = xserver;
        isShiftPressed = false;
        setKeyboardVisibilityListener(this);
    }

    private void setKeyboardVisibilityListener(final OnKeyboardVisibilityListener onKeyboardVisibilityListener) {
        final View parentView = ((ViewGroup)activity.findViewById(android.R.id.content)).getChildAt(0);
        parentView.getViewTreeObserver().addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            private boolean alreadyOpen;
            private final int defaultKeyboardHeightDP = 10;
            private final int EstimatedKeyboardDP = defaultKeyboardHeightDP + (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP ? 48 : 0);
            private final Rect rect = new Rect();

            @Override
            public void onGlobalLayout() {
                int estimatedKeyboardHeight = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, EstimatedKeyboardDP, parentView.getResources().getDisplayMetrics());
                parentView.getWindowVisibleDisplayFrame(rect);
                int heightDiff = parentView.getRootView().getHeight() - (rect.bottom - rect.top);
                boolean isShown = heightDiff >= estimatedKeyboardHeight;
                if (isShown == alreadyOpen) {
                    return;
                }
                alreadyOpen = isShown;
                onKeyboardVisibilityListener.onVisibilityChanged(isShown);
            }
        });
    }

    @Override
    public void onVisibilityChanged(boolean visible) {
        isShown = visible;
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

    public void showKeyboard() {
        isShiftPressed = false;
        isShown = true;
        keyboard.showSoftInput(view, InputMethodManager.SHOW_FORCED);
    }

    public void hideKeyboard() {
        isShiftPressed = false;
        isShown = false;
        keyboard.hideSoftInputFromWindow(view.getWindowToken(), 0);
    }

    private int getSpecialKeyCode(int keycode) {
        switch (keycode) {
            default:
                return keycode;
            case 17:
                return 15;
            case 18:
                return 10;
            case 77:
                return 9;
            case 81:
                return 70;
        }
    }

    public boolean pressKey(KeyEvent event) {
        int key;
        switch (event.getKeyCode()) {
            default:
                key = event.getKeyCode();
                if (event.isShiftPressed()) {
                    isShiftPressed = true;
                } else {
                    isShiftPressed = false;
                }
                break;
            case 17:
            case 18:
            case 77:
            case 81:
                key = getSpecialKeyCode(event.getKeyCode());
                if (event.getAction() == KeyEvent.ACTION_DOWN) {
                    isShiftPressed = true;
                } else {
                    isShiftPressed = event.isShiftPressed();
                }
                break;
        }
        return pressKey(event.getAction(), key, event.getMetaState());
    }

    public boolean pressKey(int action, int key, int metaState) {
        if (!xserver.isSystemPaused()) {
            if (isShiftPressed) {
                xserver.getWineActivity().invoke("wine_keyboard_event", xserver.getFocusedWindow().getHWND(), action, 59, metaState);
            }
            return (boolean) xserver.getWineActivity().invoke("wine_keyboard_event", xserver.getFocusedWindow().getHWND(), action, key, metaState);
        }
        return false;
    }
}
