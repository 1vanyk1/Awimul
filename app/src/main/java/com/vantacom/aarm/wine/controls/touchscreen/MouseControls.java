package com.vantacom.aarm.wine.controls.touchscreen;

import android.content.Context;
import android.graphics.PointF;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;

import androidx.core.view.GestureDetectorCompat;

import com.vantacom.aarm.LibraryManager;
import com.vantacom.aarm.wine.controls.MouseActions;
import com.vantacom.aarm.wine.controls.MouseWheelActions;
import com.vantacom.aarm.wine.xserver.Mouse;
import com.vantacom.aarm.wine.xserver.XServerManager;
import com.vantacom.aarm.wine.xserver.views.Window;

public class MouseControls extends BaseControls implements GestureDetector.OnGestureListener, GestureDetector.OnDoubleTapListener {
    private GestureDetectorCompat gDetector;
    private LibraryManager wineActivity;
    private XServerManager xserver;
    private Mouse mouse;
    private MouseWheelActions wheelActions;

    private boolean isMoving = false;
    private boolean isMultiTouch = false;
    private boolean isTripleTouch = false;
    private boolean isLongPress = false;

    private PointF pointStart, mouseStart;
    private PointF pointReal, point2;

    public MouseControls(Context context, XServerManager xserver) {
        this.wineActivity = xserver.getWineActivity();
        this.xserver = xserver;
        gDetector = new GestureDetectorCompat(context,this);
        gDetector.setOnDoubleTapListener(this);
        wheelActions = new MouseWheelActions(xserver);
        this.mouse = xserver.getCursor();
    }

    @Override
    public void updateWindow(MotionEvent event) {
        Window w = xserver.getTouchedWindow(mouse);
        xserver.changeFocus(w);
    }

    @Override
    public void stopMoving() {
        if (!isMultiTouch) {
            if (isMoving) {
                MouseActions.setLeftButtonClick(mouse.getX(), mouse.getY(), wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_UP);
                isMoving = false;
                if (isLongPress) {
                    isLongPress = false;
                }
            }
            if (xserver.getFocusedWindow() != null) {
                xserver.getFocusedWindow().setCanMove(true);
            }
        }
        isMoving = false;
        isMultiTouch = false;
        isTripleTouch = false;
        isLongPress = false;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (!xserver.isSystemPaused()) {
            int action = event.getActionMasked();
            if (event.getPointerCount() >= 2) {
                point2 = xserver.getDesktopView().getDesktopCords(event.getX(1), event.getY(1));
                try {
                    point2 = xserver.getFocusedWindow().convertDeskCordsToWin(point2.x, point2.y);
                } catch (ClassNotFoundException e) {}
            }
            PointF eventPos = xserver.getDesktopView().getDesktopCords(event.getX(), event.getY());
            PointF point = xserver.getDesktopView().getDesktopCords(event.getX(), event.getY());
            try {
                point = xserver.getFocusedWindow().convertDeskCordsToWin(point.x, point.y);
                event.setLocation(point.x, point.y);
            } catch (ClassNotFoundException e) {
                return true;
            }
            pointReal = new PointF(mouse.getX(), mouse.getY());
            try {
                pointReal = xserver.getFocusedWindow().convertDeskCordsToWin(pointReal);
            } catch (ClassNotFoundException e) {}
            switch (action) {
                default:
                    break;
                case MotionEvent.ACTION_POINTER_DOWN:
                    if (!isMoving) {
                        isMultiTouch = true;
                        if (event.getPointerCount() == 2) {
                            xserver.getResizeManager().setStartDistance(event, point2);
                            MouseActions.setLeftButtonClick(pointReal.x, pointReal.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_UP);
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
                            if (!isTripleTouch) {
                                xserver.getKeyboard().toggleKeyboard();
                            }
                        }
                    }
                    break;
                case MotionEvent.ACTION_DOWN:
                    if (!isMultiTouch) {
                        pointStart = new PointF(eventPos.x, eventPos.y);
                        mouseStart = new PointF(mouse.getX(), mouse.getY());
                        return gDetector.onTouchEvent(event);
                    }
                    break;
                case MotionEvent.ACTION_MOVE:
                    if (!isMultiTouch) {
                        float x = mouseStart.x - pointStart.x + eventPos.x;
                        float y = mouseStart.y - pointStart.y + eventPos.y;
                        mouse.setPosition(x, y);
                        pointReal = new PointF(mouse.getX(), mouse.getY());
                        try {
                            pointReal = xserver.getFocusedWindow().convertDeskCordsToWin(pointReal);
                        } catch (ClassNotFoundException e) {}
                        if (isLongPress) {
                            if (xserver.getFocusedWindow().getCanMove()) {
                                if (!isMoving) {
                                    isMoving = true;
                                    MouseActions.setLeftButtonClick(pointReal.x, pointReal.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_DOWN);
                                } else {
                                    MouseActions.setLeftButtonClick(pointReal.x, pointReal.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_MOVE);
                                }
                            }
                        } else {
                            isMoving = true;
                            MouseActions.setLeftButtonClick(pointReal.x, pointReal.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_MOVE);
                        }
                        return gDetector.onTouchEvent(event);
                    }
                    isMoving = true;
                    if (event.getPointerCount() == 2) {
                        xserver.getResizeManager().resize(event, point2);
                    }
                    break;
                case MotionEvent.ACTION_UP:
                    if (!isMultiTouch) {
                        if (isMoving) {
                            isMoving = false;
                            if (isLongPress) {
                                MouseActions.setLeftButtonClick(pointReal.x, pointReal.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_UP);
                                isLongPress = false;
                            }
                        } else {
                            if (isLongPress) {
                                isLongPress = false;
                                updateWindow(event);
                                MouseActions.singleRightButtonClick(pointReal.x, pointReal.y, wineActivity, xserver.getFocusedWindow());
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
    public boolean onSingleTapConfirmed(MotionEvent e) {
        return true;
    }

    @Override
    public boolean onDoubleTap(MotionEvent e) {
        return MouseActions.singleLeftButtonClick(pointReal.x, pointReal.y, wineActivity, xserver.getFocusedWindow());
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return true;
    }

    @Override
    public boolean onDown(MotionEvent e) {
        return true;
    }

    @Override
    public void onShowPress(MotionEvent e) {}

    @Override
    public boolean onSingleTapUp(MotionEvent event) {
        if (!isMultiTouch) {
            updateWindow(event);
            return MouseActions.singleLeftButtonClick(pointReal.x, pointReal.y, wineActivity, xserver.getFocusedWindow());
        }
        return MouseActions.setLeftButtonClick(pointReal.x, pointReal.y, wineActivity, xserver.getFocusedWindow(), MouseActions.MOUSE_MOVE);
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        if (!isTripleTouch) {
            isMoving = true;
        }
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e) {
        isLongPress = true;
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        return false;
    }
}
