package com.vantacom.aarm.wine.controls.touchscreen;

import android.view.MotionEvent;
import android.view.View;

import com.vantacom.aarm.wine.xserver.Mouse;

public abstract class BaseControls implements View.OnTouchListener {
    public abstract void updateWindow(MotionEvent event);

    public abstract void stopMoving();
}
