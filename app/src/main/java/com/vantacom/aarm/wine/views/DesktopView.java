package com.vantacom.aarm.wine.views;

import android.content.Context;
import android.graphics.PointF;
import android.util.Log;
import android.view.MotionEvent;
import android.view.ViewGroup;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.xserver.XServerManager;

public class DesktopView extends ViewGroup {
    private Window desktopWindow;
    private XServerManager xserver;
    private CustomClassManager wineActivity;
    private boolean isCreated = false;

    private float startDistance = 0f;
    private float scale = 1f;
    private float scaleView = 1f;

    private int width, height;
    private float layoutWidth, layoutHeight;
    private float screenWidth, screenHeight;

    private float pressX = 0f, pressY = 0f;

    public DesktopView(XServerManager xserver, CustomClassManager wineActivity, Context context, int hwnd, int width, int height, int layoutWidth, int layoutHeight) {
        super(context);
        this.width = width;
        this.height = height;
        this.layoutWidth = layoutWidth;
        this.layoutHeight = layoutHeight;
        this.xserver = xserver;
        this.wineActivity = wineActivity;
        createDesktopWindow(hwnd);
    }

    public void createDesktopWindow(int hwnd) {
        desktopWindow = new Window(xserver, hwnd, null, 1.0F);
        addView(desktopWindow.createWindowView());
        desktopWindow.getGroup(true).bringToFront();
    }

    public void destroy() {
        desktopWindow.destroy();
        desktopWindow = null;
        xserver = null;
        wineActivity = null;
    }

    public Window getDesktopWindow() {
        return desktopWindow;
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {}

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        if (!isCreated) {
            isCreated = true;
            screenWidth = w;
            screenHeight = h;
            scaleView = Math.min(((float)layoutWidth) / width, ((float)layoutHeight) / height);
            super.onSizeChanged(width, height, oldw, oldh);
            wineActivity.invoke("wine_desktop_changed", width, height);
            resizeLayout(1f);
            scale = getScaleX();
        }
    }

    private float getDistance(float x1, float y1, float x2, float y2) {
        double x = x2 - x1, y = y2 - y1;
        return (float)Math.sqrt(x * x + y * y);
    }

    public void setStartDistance(MotionEvent event, Window window) {
        scale = getScaleX();
        pressX = (int)event.getX(0);
        pressY = (int)event.getY(0);
        float eventX1, eventY1;
        PointF point = xserver.getDesktopView().getDesktopCords(event.getX(1), event.getY(1));
        try {
            point = xserver.convertDeskCordsToWin(point.x, point.y, window);
        } catch (ClassNotFoundException e) {}
        eventX1 = point.x;
        eventY1 = point.y;
        startDistance = getDistance(pressX, pressY, eventX1, eventY1);
    }

    public PointF getDesktopCords() {
        float x = getX();
        float y = getY();
        return new PointF(x, y);
    }

    private float getRealX() {
        return getX() - layoutWidth * scale / 2f + layoutWidth / 2f;
    }

    private float getRealY() {
        return getY() - layoutHeight * scale / 2f + layoutHeight / 2f;
    }

    private void setRealX(float x) {
        setX(x + layoutWidth / 2f * (scale - 1));
    }

    private void setRealY(float y) {
        setY(y + layoutHeight / 2f * (scale - 1));
    }

    public PointF getDesktopCords(float eventX, float eventY) {
        float x = (eventX - getRealX()) / scale;
        float y = (eventY - getRealY()) / scale;
        return new PointF(x, y);
    }

    public void resizeLayout(float scale) {
        if (scale < 1.05f) {
            setScaleX(scaleView);
            setScaleY(scaleView);
            if (screenWidth > layoutWidth) {
                setX((layoutWidth - width) / 2f * scaleView - (screenWidth - layoutWidth) / 10f / scaleView);
            } else {
                setX((layoutWidth - width) / 2f * scaleView);
            }
            if (screenHeight > layoutHeight) {
                setY((layoutHeight - height) / 2f * scaleView - (screenHeight - layoutHeight) / 10f / scaleView);
            } else {
                setY((layoutHeight - height) / 2f * scaleView);
            }
        } else if (5f < scale) {
            setScaleX(scaleView * 5f);
            setScaleY(scaleView * 5f);
        } else {
            setScaleX(scaleView * scale);
            setScaleY(scaleView * scale);
        }
    }

    public void resize(MotionEvent event, Window window) {
        float eventX1, eventY1;
        PointF point = xserver.getDesktopView().getDesktopCords(event.getX(1), event.getY(1));
        try {
            point = xserver.convertDeskCordsToWin(point.x, point.y, window);
        } catch (ClassNotFoundException e) {}
        eventX1 = point.x;
        eventY1 = point.y;
        scale = getScaleX();
        float distance = getDistance(event.getX(0), event.getY(0), eventX1, eventY1);
        float newSize = distance / startDistance * scale / scaleView;
        setX(event.getX(0) - pressX + getX());
        setY(event.getY(0) - pressY + getY());
//        float newSize = 2f;
//        setRealX(-100f);
//        setRealY(40f);
        resizeLayout(newSize);
    }
}
