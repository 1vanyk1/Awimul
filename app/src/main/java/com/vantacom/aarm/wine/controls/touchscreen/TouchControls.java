package com.vantacom.aarm.wine.controls.touchscreen;

import android.content.Context;
import android.graphics.PointF;
import android.view.GestureDetector;
import android.view.InputDevice;
import android.view.MotionEvent;
import android.view.View;

import androidx.core.view.GestureDetectorCompat;

import com.vantacom.aarm.wine.controls.MouseActions;
import com.vantacom.aarm.wine.controls.MouseWheelActions;
import com.vantacom.aarm.wine.xserver.Mouse;
import com.vantacom.aarm.wine.xserver.views.Window;
import com.vantacom.aarm.wine.xserver.XServerManager;

public class TouchControls extends BaseControls implements GestureDetector.OnGestureListener, GestureDetector.OnDoubleTapListener {
    private GestureDetectorCompat gDetector;
    private MouseWheelActions wheelActions;

    private boolean isMoving = false;
    private boolean isMultiTouch = false;
    private boolean isTripleTouch = false;
    private boolean isLongPress = false;

    private PointF point2;

    public TouchControls(Context context, XServerManager xserver) {
        this.wineActivity = xserver.getWineActivity();
        this.xserver = xserver;
        gDetector = new GestureDetectorCompat(context,this);
        gDetector.setOnDoubleTapListener(this);
        wheelActions = new MouseWheelActions(xserver);
        this.mouse = xserver.getCursor();
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
        try {
            mouse.setPosition(xserver.getFocusedWindow().convertWinCordsToDesk(event.getX(), event.getY()));
        } catch (ClassNotFoundException e) {}
    }

    @Override
    public void stopMoving() {
        if (!isMultiTouch) {
            if (isMoving) {
                MouseActions.setLeftButtonClick(mouse.getX(), mouse.getY(), xserver.getWM(), MouseActions.MOUSE_UP);
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

    protected boolean onTouchTouchScreen(MotionEvent event) {
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
                                try {
                                    mouse.setPosition(xserver.getFocusedWindow().convertWinCordsToDesk(event.getX(), event.getY()));
                                } catch (ClassNotFoundException e) {}
                                MouseActions.setLeftButtonClick(event.getX(), event.getY(), xserver.getWM(), MouseActions.MOUSE_DOWN);
                            } else {
                                try {
                                    mouse.setPosition(xserver.getFocusedWindow().convertWinCordsToDesk(event.getX(), event.getY()));
                                } catch (ClassNotFoundException e) {}
                                MouseActions.setLeftButtonClick(event.getX(), event.getY(), xserver.getWM(), MouseActions.MOUSE_MOVE);
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
//                        MouseActions.setLeftButtonClick(event.getX(), event.getY(), xserver.getWM(), MouseActions.MOUSE_UP);
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
                        if (!isTripleTouch) {
                            xserver.getKeyboard().toggleKeyboard();
                        }
                    }
                }
                break;
            case MotionEvent.ACTION_UP:
                if (!isMultiTouch) {
                    if (isMoving) {
//                        MouseActions.setLeftButtonClick(event.getX(), event.getY(), xserver.getWM(), MouseActions.MOUSE_UP);
                        isMoving = false;
                        if (isLongPress) {
                            isLongPress = false;
                        }
                    } else {
                        if (isLongPress) {
                            isLongPress = false;
                            updateWindow(event);
                            MouseActions.singleRightButtonClick(event.getX(), event.getY(), xserver.getWM());
                        }
                    }
                    xserver.getFocusedWindow().setCanMove(true);
                    return gDetector.onTouchEvent(event);
                }
                isMultiTouch = false;
                isTripleTouch = false;
                break;
        }
        return true;
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        if (!xserver.isSystemPaused()) {
            if ((InputDevice.getDevice(event.getDeviceId()).getSources() & InputDevice.SOURCE_TOUCHSCREEN) == InputDevice.SOURCE_TOUCHSCREEN) {
                if (inputDevice != INPUT_TOUCH_SCREEN) {
                    inputDevice = INPUT_TOUCH_SCREEN;
                    isMultiTouch = false;
                    isTripleTouch = false;
                }
                return onTouchTouchScreen(event);
            } else if ((InputDevice.getDevice(event.getDeviceId()).getSources() & InputDevice.SOURCE_MOUSE) == InputDevice.SOURCE_MOUSE) {
                if (inputDevice != INPUT_MOUSE) {
                    inputDevice = INPUT_MOUSE;
                    isMultiTouch = false;
                    isTripleTouch = false;
                }
                return onTouchMouse(event);
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
            return MouseActions.singleLeftButtonClick(event.getX(), event.getY(), xserver.getWM());
        }
        return true;
//        return MouseActions.setLeftButtonClick(event.getX(), event.getY(), xserver.getWM(), MouseActions.MOUSE_MOVE);
    }

    @Override
    public boolean onScroll(MotionEvent event1, MotionEvent event2, float distanceX, float distanceY) {
        if (!isTripleTouch) {
            if (isMoving) {
                wheelActions.move(mouse, event2);
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
        return MouseActions.singleLeftButtonClick(event.getX(), event.getY(), xserver.getWM());
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return false;
    }
}
