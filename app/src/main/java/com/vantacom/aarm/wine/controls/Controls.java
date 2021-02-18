package com.vantacom.aarm.wine.controls;

import android.content.Context;
import android.graphics.PointF;
import android.util.Log;
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
    private MouseWheelActions wheelActions;

    private boolean isMoving = false;
    private boolean isMultiTouch = false;
    private boolean isTripleTouch = false;
    private boolean isLongPress = false;

    private PointF point1, point2, mousePos;

    public Controls(Context context, XServerManager xserver) {
        this.wineActivity = xserver.getWineActivity();
        this.xserver = xserver;
        gDetector = new GestureDetectorCompat(context,this);
        gDetector.setOnDoubleTapListener(this);
        wheelActions = new MouseWheelActions(xserver);
        mousePos = new PointF(1, 1);
    }

    public void updateWindow(MotionEvent event) {
        Window w = xserver.getTouchedWindow(point1.x, point1.y);
        PointF point = xserver.getDesktopView().getDesktopCords(point1.x, point1.y);
        xserver.changeFocus(w);
        try {
            point = xserver.getFocusedWindow().convertDeskCordsToWin(point.x, point.y);
            event.setLocation(point.x, point.y);
        } catch (ClassNotFoundException e) {
            event.setLocation(point.x, point.y);
        }
        mousePos = new PointF(event.getX(), event.getY());
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (xserver.getFocusedWindow() != null) {
            Log.e("w", String.valueOf(xserver.getFocusedWindow().getHWND()));
        }
        if (!xserver.isSystemPaused()) {
            int action = event.getActionMasked();
            point1 = new PointF(event.getX(), event.getY());
            if (event.getPointerCount() >= 2) {
                point2 = xserver.getDesktopView().getDesktopCords(event.getX(1), event.getY(1));
                try {
                    point2 = xserver.getFocusedWindow().convertDeskCordsToWin(point2.x, point2.y);
                } catch (ClassNotFoundException e) {}
            }
            PointF point = xserver.getDesktopView().getDesktopCords(event.getX(), event.getY());
            try {
                point = xserver.getFocusedWindow().convertDeskCordsToWin(point.x, point.y);
                event.setLocation(point.x, point.y);
            } catch (ClassNotFoundException e) {
                return true;
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
                            if (xserver.getFocusedWindow().getCanMove()) {
                                if (!isMoving) {
                                    isMoving = true;
                                    updateWindow(event);
                                    MouseActions.setLeftButtonClick(mousePos.x, mousePos.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_DOWN);
                                } else {
                                    mousePos = new PointF(event.getX(), event.getY());
                                    MouseActions.setLeftButtonClick(mousePos.x, mousePos.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_MOVE);
                                }
                            }

                        }
                        return gDetector.onTouchEvent(event);
                    }
                    isMoving = true;
                    if (event.getPointerCount() == 2) {
                        xserver.getResizeManager().resize(event, point2);
                    }
                    break;
                case MotionEvent.ACTION_POINTER_DOWN:
                    if (!isMoving) {
                        isMultiTouch = true;
                        if (event.getPointerCount() == 2) {
                            xserver.getResizeManager().setStartDistance(event, point2);
                            MouseActions.setLeftButtonClick(mousePos.x, mousePos.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_UP);
                            event.setAction(MotionEvent.ACTION_UP);
                            gDetector.onTouchEvent(event);
                        } else if (event.getPointerCount() == 3) {
                            isTripleTouch = true;
                            xserver.toggleTopBar();
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
                            MouseActions.setLeftButtonClick(mousePos.x, mousePos.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_UP);
                            isMoving = false;
                            if (isLongPress) {
                                isLongPress = false;
                            }
                        } else {
                            if (isLongPress) {
                                isLongPress = false;
                                updateWindow(event);
                                MouseActions.singleRightButtonClick(mousePos.x, mousePos.y, wineActivity, xserver.getFocusedWindow());
                            }
                        }
                        xserver.getFocusedWindow().setCanMove(true);
                        return gDetector.onTouchEvent(event);
                    }
                    isMultiTouch = false;
                    isTripleTouch = false;
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
            updateWindow(event);
            return MouseActions.singleLeftButtonClick(mousePos.x, mousePos.y, wineActivity, xserver.getFocusedWindow());
        }
        return MouseActions.setLeftButtonClick(mousePos.x, mousePos.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_MOVE);
    }

    @Override
    public boolean onScroll(MotionEvent event1, MotionEvent event2, float distanceX, float distanceY) {
        if (!isTripleTouch) {
            if (isMoving) {
                wheelActions.move(mousePos, event2);
            } else {
                wheelActions.setStartY(event1);
                isMoving = true;
            }
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
        return MouseActions.singleLeftButtonClick(mousePos.x, mousePos.y, wineActivity, xserver.getFocusedWindow());
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return false;
    }

}
