package com.vantacom.aarm.wine;

import android.content.Context;
import android.view.MotionEvent;
import android.view.ViewGroup;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.views.Window;

public class MainView extends ViewGroup {
    private Window desktopWindow;
    private WineActivity activity;
    private CustomClassManager wineActivity;
    private boolean isCreated = false;

    private float startDistance = 0f;
    private float scale = 1f;
    private float scaleView = 1f;

    private int width, height;
    private int layoutWidth, layoutHeight;
    private int screenWidth, screenHeight;

    private float pressX = 0f, pressY = 0f;

    public MainView(Context context) {
        super(context);
    }

    public MainView(WineActivity activity, CustomClassManager wineActivity, Context context, int desktopView) {
        super(context);
        this.activity = activity;
        this.wineActivity = wineActivity;
        createDesktopWindow(desktopView);
    }

    public void createDesktopWindow(int desktopView) {
        desktopWindow = new Window(activity, wineActivity, desktopView, null, 1.0F);
        addView(desktopWindow.createWindowView());
        desktopWindow.getGroup(true).bringToFront();
    }

    public void destroy() {
        desktopWindow.destroy();
        desktopWindow = null;
        activity = null;
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
            width = activity.getDesktopWidth();
            height = activity.getDesktopHeight();
            layoutWidth = activity.getScreenWidth();
            layoutHeight = activity.getScreenHeight();
            screenWidth = w;
            screenHeight = h;
            scaleView = Math.min(((float)layoutWidth) / width, ((float)layoutHeight) / height);
            super.onSizeChanged(width, height, oldw, oldh);
            wineActivity.invoke("wine_desktop_changed", width, height);
            resizeLayout(1f);
        }
    }

    private float getDistance(float x1, float y1, float x2, float y2) {
        double x = x2 - x1, y = y2 - y1;
        return (float)Math.sqrt(x * x + y * y);
    }

    public void setStartDistance(MotionEvent event) {
        scale = getScaleX();
        pressX = event.getX(0);
        pressY = event.getY(0);
        startDistance = getDistance(pressX, pressY, event.getX(1), event.getY(1));
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
//            setX((layoutWidth - width) / 2f * scaleView);
        } else if (5f < scale) {
            setScaleX(scaleView * 5f);
            setScaleY(scaleView * 5f);
        } else {
            setScaleX(scaleView * scale);
            setScaleY(scaleView * scale);
        }
    }

    public void resize(MotionEvent event) {
        scale = getScaleX();
        float distance = getDistance(event.getX(0), event.getY(0), event.getX(1), event.getY(1));
        float newSize = distance / startDistance * scale / scaleView;
        setX(event.getX(0) - pressX + getX());
        setY(event.getY(0) - pressY + getY());
        resizeLayout(newSize);
    }
}
