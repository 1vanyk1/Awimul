package com.vantacom.aarm.wine.xserver;

import android.graphics.PointF;
import android.view.MotionEvent;

public class ResizeManager {
    private float pressX = 0f, pressY = 0f;
    private float startDistance = 0f;
    private float scale = 1f;

    private XServerManager xserver;

    public ResizeManager(XServerManager xserver) {
        this.xserver = xserver;
    }

    private float getDistance(float x1, float y1, float x2, float y2) {
        double x = x2 - x1, y = y2 - y1;
        return (float)Math.sqrt(x * x + y * y);
    }

    public void setStartDistance(MotionEvent event, PointF secondPoint) {
        pressX = event.getX(0);
        pressY = event.getY(0);
        startDistance = getDistance(pressX, pressY, secondPoint.x, secondPoint.y);
    }

    public void resize(MotionEvent event, PointF secondPoint) {
        float eventX1, eventY1;
        eventX1 = secondPoint.x;
        eventY1 = secondPoint.y;
        float distance = getDistance(event.getX(0), event.getY(0), eventX1, eventY1);
        float newSize = distance / startDistance * scale;
        scale = xserver.getDesktopView().resizeLayout(newSize, event.getX(0) - pressX, event.getY(0) - pressY);
    }

    public float getScale() {
        return scale;
    }
}
