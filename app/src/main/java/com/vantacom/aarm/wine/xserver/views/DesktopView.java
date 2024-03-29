package com.vantacom.aarm.wine.xserver.views;

import android.content.Context;
import android.graphics.PointF;
import android.util.Log;
import android.view.ViewGroup;

import com.vantacom.aarm.wine.xserver.XServerManager;

public class DesktopView extends ViewGroup {
    private Window desktopWindow;
    private XServerManager xserver;
    private boolean isCreated = false;

    private float scaleView = 1f;

    private int width, height;
    private float layoutWidth, layoutHeight;
    private float screenWidth, screenHeight;

    public DesktopView(XServerManager xserver, Context context, int hwnd, int width, int height, int layoutWidth, int layoutHeight) {
        super(context);
        this.width = width;
        this.height = height;
        this.layoutWidth = layoutWidth;
        this.layoutHeight = layoutHeight;
        this.xserver = xserver;
        createDesktopWindow(hwnd);
        ScreenGroupView screenGroupView = new ScreenGroupView(xserver, xserver.getWM());
        addView(screenGroupView);
        screenGroupView.setLayout(0, 0, width, height);
        screenGroupView.createContentView().layout(0, 0, width, height);
        screenGroupView.setScale(1);
        addView(xserver.getCursor().getView());
    }

    public void createDesktopWindow(int hwnd) {
        desktopWindow = new Window(xserver, hwnd, null, 1.0F);
        addView(desktopWindow.createWindowView());
        desktopWindow.getGroup(true).bringToFront();
    }

    public void disconnectCursor() {
        removeView(xserver.getCursor().getView());
    }

    public void destroy() {
        desktopWindow.destroy();
        desktopWindow = null;
        xserver = null;
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
            scaleView = Math.min(layoutWidth / width, layoutHeight / height);
            super.onSizeChanged(width, height, oldw, oldh);
            resizeLayout(1f);
        }
    }

    private float getRealX() {
        return getX() - screenWidth * xserver.getScale() * scaleView / 2f + screenWidth / 2f;
    }

    private float getRealY() {
        return getY() - screenHeight * xserver.getScale() * scaleView / 2f + screenHeight / 2f;
    }

    public PointF getDesktopCords(float eventX, float eventY) {
        float x, y;
        x = (eventX - getRealX()) / xserver.getScale() / scaleView;
        y = (eventY - getRealY()) / xserver.getScale() / scaleView;
        return new PointF(x, y);
    }

    public float resizeLayout(float scale) {
        if (scale < 1.05f) {
            setScaleX(scaleView);
            setScaleY(scaleView);
            if (screenWidth > layoutWidth) {
                setX((layoutWidth - width) / 2f);
            } else {
                setX((layoutWidth - width) / 2f * scaleView);
            }
            if (screenHeight > layoutHeight) {
                setY((layoutHeight - height) / 2f);
            } else {
                setY((layoutHeight - height) / 2f * scaleView);
            }
            return 1f;
        } else if (5f < scale) {
            setScaleX(scaleView * 5f);
            setScaleY(scaleView * 5f);
            return 5f;
        } else {
            setScaleX(scaleView * scale);
            setScaleY(scaleView * scale);
            return scale;
        }
    }

    public float resizeLayout(float scale, float x, float y) {
        setX(x + getX());
        setY(y + getY());
        return resizeLayout(scale);
    }
}
