package com.vantacom.aarm.wine.controls;

import android.graphics.PointF;
import android.view.MotionEvent;

import com.vantacom.aarm.wine.xserver.XServerManager;

public class MouseWheelActions {
    private float y;
    private XServerManager xserver;

    public MouseWheelActions(XServerManager xserver) {
        this.xserver = xserver;
    }

    public void setStartY(MotionEvent event) {
        this.y = event.getY();
    }

    public void move(PointF mousePos, MotionEvent event) {
        float eventY = event.getY();
        if (eventY + 5 < y) {
            MouseActions.scroll(mousePos.x, mousePos.y, xserver.getWineActivity(), xserver.getFocusedWindow(), -1);
            y = eventY;
        } else if (eventY - 5 > y) {
            MouseActions.scroll(mousePos.x, mousePos.y, xserver.getWineActivity(), xserver.getFocusedWindow(), 1);
            y = eventY;
        }
    }
}
