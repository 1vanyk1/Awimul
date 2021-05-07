package com.vantacom.aarm.wine.controls;

import android.view.MotionEvent;

import com.vantacom.aarm.wine.xserver.Mouse;
import com.vantacom.aarm.wine.xserver.XServerManager;

public class MouseWheelActions {
    private float y;
    private XServerManager xserver;

    public MouseWheelActions(XServerManager xserver) {
        this.xserver = xserver;
    }

    public void setStartY(MotionEvent event) {
        try {
            this.y = event.getY();
        } catch (Exception e) {
            this.y = 0;
        }
    }

    public void move(Mouse mouse, MotionEvent event) {
        float eventY = event.getY();
        if (eventY + 5 < y) {
            MouseActions.scroll(mouse.getX(), mouse.getY(), xserver.getWineActivity(), xserver.getFocusedWindow(), -1);
            y = eventY;
        } else if (eventY - 5 > y) {
            MouseActions.scroll(mouse.getX(), mouse.getY(), xserver.getWineActivity(), xserver.getFocusedWindow(), 1);
            y = eventY;
        }
    }
}
