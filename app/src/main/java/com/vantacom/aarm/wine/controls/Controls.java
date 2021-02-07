package com.vantacom.aarm.wine.controls;

import android.content.Context;
import android.graphics.PointF;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;

import androidx.core.view.GestureDetectorCompat;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.xserver.views.Window;
import com.vantacom.aarm.wine.xserver.XServerManager;

public class Controls implements View.OnTouchListener, GestureDetector.OnGestureListener, GestureDetector.OnDoubleTapListener{
    private GestureDetectorCompat gDetector;
    private CustomClassManager wineActivity;
    private XServerManager xserver;
    private Window window;
    private MouseWheelActions wheelActions;

    private boolean isMoving = false;
    private boolean isMultiTouch = false;
    private boolean isLongPress = false;

    public Controls(Context context, XServerManager xserver) {
        this.wineActivity = xserver.getWineActivity();
        this.xserver = xserver;
        gDetector = new GestureDetectorCompat(context,this);
        gDetector.setOnDoubleTapListener(this);
        wheelActions = new MouseWheelActions(xserver);
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (!xserver.isSystemPaused()) {
            PointF secondPoint = null;
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
                if (event.getPointerCount() >= 2) {
                    secondPoint = xserver.getDesktopView().getDesktopCords(event.getX(1), event.getY(1));
                    try {
                        secondPoint = xserver.convertDeskCordsToWin(secondPoint.x, secondPoint.y, window);
                    } catch (ClassNotFoundException e) {}
                }
                PointF point = xserver.getDesktopView().getDesktopCords(event.getX(), event.getY());
                try {
                    point = xserver.convertDeskCordsToWin(point.x, point.y, window);
                    event.setLocation(point.x, point.y);
                } catch (ClassNotFoundException e) {
                    return true;
                }
            }
            switch (action) {
                default:
                    if (!isMultiTouch) {
                        return gDetector.onTouchEvent(event);
                    }
                    break;
                case MotionEvent.ACTION_MOVE:
                    if (!isMultiTouch) {
                        if (isLongPress) {
                            if (window.getCanMove()) {
                                if (!isMoving) {
                                    isMoving = true;
                                    MouseActions.setLeftButtonClick(event, wineActivity, window, MouseActions.MOUSE_DOWN);
                                } else {
                                    MouseActions.setLeftButtonClick(event, wineActivity, window, MouseActions.MOUSE_MOVE);
                                }
                            }

                        }
                        return gDetector.onTouchEvent(event);
                    }
                    isMoving = true;
                    if (event.getPointerCount() == 2) {
                        xserver.getResizeManager().resize(event, secondPoint);
                    }
                    break;
                case MotionEvent.ACTION_POINTER_DOWN:
                    if (!isMoving) {
                        isMultiTouch = true;
                        if (event.getPointerCount() == 2) {
                            xserver.getResizeManager().setStartDistance(event, secondPoint);
                            MouseActions.setLeftButtonClick(event, wineActivity, window, MouseActions.MOUSE_UP);
                            event.setAction(MotionEvent.ACTION_UP);
                            gDetector.onTouchEvent(event);
                        }
                    }
                    break;
                case MotionEvent.ACTION_POINTER_UP:
                    if (isMultiTouch) {
                        if (isMoving) {
                            isMoving = false;
                        } else {
                            xserver.getKeyboard().toggleKeyboard();
                        }
                    }
                    break;
                case MotionEvent.ACTION_UP:
                    if (!isMultiTouch) {
                        if (isMoving) {
                            MouseActions.setLeftButtonClick(event, wineActivity, window, MouseActions.MOUSE_UP);
                            isMoving = false;
                            if (isLongPress) {
                                isLongPress = false;
                            }
                        } else {
                            if (isLongPress) {
                                isLongPress = false;
                                MouseActions.singleRightButtonClick(event, wineActivity, window);
                            }
                        }
                        window.setCanMove(true);
                        return gDetector.onTouchEvent(event);
                    }
                    isMultiTouch = false;
                    break;
            }
        }
        return true;
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
        return MouseActions.setLeftButtonClick(event, wineActivity, window, MouseActions.MOUSE_MOVE);
    }

    @Override
    public boolean onScroll(MotionEvent event1, MotionEvent event2, float distanceX, float distanceY) {
        if (isMoving) {
            wheelActions.move(event2);
        } else {
            wheelActions.setStartY(event1, window);
            isMoving = true;
        }
        return true;
    }

    @Override
    public void onLongPress(MotionEvent event) {
        isLongPress = true;
    }

    @Override
    public boolean onFling(MotionEvent event1, MotionEvent event2, float velocityX, float velocityY) {
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
