package com.vantacom.aarm.wine.controls;

import android.content.Context;
import android.graphics.PointF;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;

import androidx.core.view.GestureDetectorCompat;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.views.Window;
import com.vantacom.aarm.wine.xserver.XServerManager;

public class Controls implements View.OnTouchListener, GestureDetector.OnGestureListener, GestureDetector.OnDoubleTapListener{
    private GestureDetectorCompat gDetector;
    private CustomClassManager wineActivity;
    private XServerManager xserver;
    private Window window;

    private boolean isMoving = false;
    private boolean isMultiTouch = false;

    public Controls(Context context, XServerManager xserver) {
        this.wineActivity = xserver.getWineActivity();
        this.xserver = xserver;
        gDetector = new GestureDetectorCompat(context,this);
        gDetector.setOnDoubleTapListener(this);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (!xserver.isSystemPaused()) {
            int action = event.getActionMasked();
            if (MotionEvent.ACTION_DOWN == action) {
                Window w = xserver.getTouchedWindow(event.getX(), event.getY());
                if (window == null) {
                    window = xserver.getDesktopView().getDesktopWindow();
                }
                PointF point = xserver.getDesktopView().getDesktopCords(event.getX(), event.getY());
                try {
                    point = xserver.convertDeskCordsToWin(point.x, point.y, window);
                    event.setLocation(point.x, point.y);
                    if (window != w) {
                        window = w;
                    }
                } catch (ClassNotFoundException e) {
                    event.setLocation(point.x, point.y);
                    if (window != w) {
                        window = w;
                    }
                }
                if (!isMultiTouch) {
                    return gDetector.onTouchEvent(event);
                }
            }
            else {
                PointF point = xserver.getDesktopView().getDesktopCords(event.getX(), event.getY());
                try {
                    point = xserver.convertDeskCordsToWin(point.x, point.y, window);
                    event.setLocation(point.x, point.y);
                } catch (ClassNotFoundException e) {
                    return false;
                }
                Log.e("onTouchEvent", String.valueOf(window.getHWND()));
                Log.e("onTouchEvent", String.valueOf(event.getActionMasked()));
                Log.e("point", String.format("%f %f", point.x, point.y));
                event.setLocation(point.x, point.y);
            }
            switch (action) {
                default:
                    if (!isMultiTouch) {
                        return gDetector.onTouchEvent(event);
                    }
                    break;
                case MotionEvent.ACTION_MOVE:
                    if (!isMultiTouch) {
                        return gDetector.onTouchEvent(event);
                    }
                    isMoving = true;
                    if (event.getPointerCount() == 2) {
                        xserver.getDesktopView().resize(event, window);
                    }
                    break;
                case MotionEvent.ACTION_POINTER_DOWN:
                    isMultiTouch = true;
                    if (event.getPointerCount() == 2) {
                        xserver.getDesktopView().setStartDistance(event, window);
                        event.setAction(MotionEvent.ACTION_UP);
                        gDetector.onTouchEvent(event);
                    }
                    break;
                case MotionEvent.ACTION_POINTER_UP:
                    if (isMoving) {
                        isMoving = false;
                    } else {
                        xserver.getKeyboard().toggleKeyboard();
                    }
                    break;
                case MotionEvent.ACTION_UP:
                    if (!isMultiTouch) {
                        if (isMoving) {
                            MouseActions.setLeftButtonClick(event, wineActivity, window, 1);
                            isMoving = false;
                        }
                        window.setCanMove(true);
                        return gDetector.onTouchEvent(event);
                    }
                    isMultiTouch = false;
                    break;
            }
        }
        return false;
    }

    @Override
    public boolean onDown(MotionEvent event) {
        return true;
    }

    @Override
    public void onShowPress(MotionEvent event) {}

    @Override
    public boolean onSingleTapUp(MotionEvent event) {
        if (!isMultiTouch) {
            xserver.getKeyboard().setHWND(window.getHWND());
            return MouseActions.singleLeftButtonClick(event, wineActivity, window);
        }
        return MouseActions.setLeftButtonClick(event, wineActivity, window, 2);
    }

    @Override
    public boolean onScroll(MotionEvent event1, MotionEvent event2, float distanceX, float distanceY) {
        if (window.getCanMove()) {
            if (!isMoving) {
                isMoving = true;
                MouseActions.setLeftButtonClick(event1, wineActivity, window, 0);
            } else {
                MouseActions.setLeftButtonClick(event2, wineActivity, window, 2);
            }
        }
        return true;
    }

    @Override
    public void onLongPress(MotionEvent event) {
        MouseActions.singleRightButtonClick(event, wineActivity, window);
    }

    @Override
    public boolean onFling(MotionEvent event1, MotionEvent event2, float velocityX, float velocityY) {
        isMoving = false;
        window.setCanMove(true);
        return false;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent event) {
        return true;
    }

    @Override
    public boolean onDoubleTap(MotionEvent event) {
        return false;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent event) {
        return MouseActions.singleLeftButtonClick(event, wineActivity, window);
    }
}
