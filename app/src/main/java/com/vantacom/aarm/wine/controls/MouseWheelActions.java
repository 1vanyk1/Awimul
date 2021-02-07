package com.vantacom.aarm.wine.controls;

import android.view.MotionEvent;

import com.vantacom.aarm.wine.xserver.views.Window;
import com.vantacom.aarm.wine.xserver.XServerManager;

public class MouseWheelActions {
    private float y;
    private Window window;
    private XServerManager xserver;

    public MouseWheelActions(XServerManager xserver) {
        this.xserver = xserver;
    }

    public void setStartY(MotionEvent event, Window w) {
        this.y = event.getY();
        window = w;
    }

    public void move(MotionEvent event) {
        float eventY = event.getY();
        if (eventY + 5 < y) {
            MouseActions.scroll(event, xserver.getWineActivity(), window, -1);
            y = eventY;
        } else if (eventY - 5 > y) {
            MouseActions.scroll(event, xserver.getWineActivity(), window, 1);
            y = eventY;
        }
    }
}
