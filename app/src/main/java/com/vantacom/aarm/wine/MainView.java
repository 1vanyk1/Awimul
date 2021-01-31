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

//    private boolean isMoving = false;
//    private boolean isMultiTouch = false;
//    private Window window;
//    private boolean isWindowSet = false;
//    private float eventX, eventY;
//    private float startEventX, startEventY;
//
//    public boolean checkWindow(Window window) {
//        return this.window == window;
//    }
//
//    public void setCurrentWindow(Window window, MotionEvent event, float x, float y) {
//        if (!isWindowSet) {
//            isWindowSet = true;
//            this.window = window;
//            startEventX = event.getX();
//            startEventY = event.getY();
//            eventX = event.getX();
//            eventY = event.getY();
//        }
//    }
//
//    @Override
//    public boolean onSingleTapConfirmed(MotionEvent event) {
//        return false;
//    }
//
//    @Override
//    public boolean onDoubleTap(MotionEvent event) {
//        return false;
//    }
//
//    @Override
//    public boolean onDoubleTapEvent(MotionEvent event) {
//        return MouseActions.singleLeftButtonClick(event, wineActivity, window);
//    }
//
//    @Override
//    public boolean onDown(MotionEvent event) {
//        return true;
//    }
//
//    @Override
//    public void onShowPress(MotionEvent event) {
//
//    }
//
//    @Override
//    public boolean onSingleTapUp(MotionEvent event) {
//        if (!isMultiTouch) {
//            activity.getKeyboard().setHWND(window.getHWND());
//            return MouseActions.singleLeftButtonClick(event, wineActivity, window);
//        }
//        return MouseActions.setLeftButtonClick(event, wineActivity, window, 2);
//    }
//
//    @Override
//    public boolean onScroll(MotionEvent event1, MotionEvent event2, float distanceX, float distanceY) {
//        if (window.getCanMove()) {
//            if (!isMoving) {
//                isMoving = true;
//                MouseActions.setLeftButtonClick(event1, wineActivity, window, 0);
//            } else {
//                MouseActions.setLeftButtonClick(event2, wineActivity, window, 2);
//            }
//        }
//        return true;
//    }
//
//    @Override
//    public void onLongPress(MotionEvent event) {
//        MouseActions.singleRightButtonClick(event, wineActivity, window);
//    }
//
//    @Override
//    public boolean onFling(MotionEvent event1, MotionEvent event2, float velocityX, float velocityY) {
//        isMoving = false;
//        window.setCanMove(true);
//        return MouseActions.setLeftButtonClick(event2, wineActivity, window, 1);
//    }
//
//    public boolean isMoving() {
//        return isMoving;
//    }
//
//    public void setMoving(boolean moving) {
//        isMoving = moving;
//    }
//
//    public boolean isMultiTouch() {
//        return isMultiTouch;
//    }
//
//    public void setMultiTouch(boolean multiTouch) {
//        isMultiTouch = multiTouch;
//    }
//
//    @Override
//    public boolean onTouchEvent(MotionEvent event) {
//        Log.e("getX", String.valueOf(event.getX()));
//        Log.e("eventX", String.valueOf(eventX));
//        float resX = startEventX + event.getX() - eventX;
//        float resY = startEventY + event.getY() - eventY;
//        eventX = event.getX();
//        eventY = event.getY();
//        event.setLocation(resX, resY);
//        if (!activity.isSystemPaused()) {
//            int action = event.getActionMasked();
//            Log.e("getHWND", String.valueOf(window.getHWND()));
//            switch (action) {
//                default:
//                    if (!isMultiTouch) {
//                        return gDetector.onTouchEvent(event);
//                    }
//                    break;
//                case MotionEvent.ACTION_MOVE:
//                    if (!isMultiTouch) {
//                        return gDetector.onTouchEvent(event);
//                    }
//                    isMoving = true;
//                    if (event.getPointerCount() == 2) {
//                        resize(event);
//                    }
//                    break;
//                case MotionEvent.ACTION_POINTER_DOWN:
//                    isMultiTouch = true;
//                    if (event.getPointerCount() == 2) {
//                        setStartDistance(event);
//                        event.setAction(MotionEvent.ACTION_UP);
//                        gDetector.onTouchEvent(event);
//                    }
//                    break;
//                case MotionEvent.ACTION_POINTER_UP:
//                    if (isMoving) {
//                        isMoving = false;
//                    } else {
//                        activity.getKeyboard().toggleKeyboard();
//                    }
//                    break;
//                case MotionEvent.ACTION_UP:
//                    isWindowSet = false;
//                    if (!isMultiTouch) {
//                        return gDetector.onTouchEvent(event);
//                    }
//                    isMultiTouch = false;
//                    break;
//            }
//        }
//        return true;
//    }
}
