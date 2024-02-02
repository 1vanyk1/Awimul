package com.vantacom.aarm.wine.controls.touchscreen;

import android.graphics.PointF;
import android.view.MotionEvent;
import android.view.View;

import com.vantacom.aarm.LibraryManager;
import com.vantacom.aarm.wine.controls.MouseActions;
import com.vantacom.aarm.wine.xserver.Mouse;
import com.vantacom.aarm.wine.xserver.XServerManager;

public abstract class BaseControls implements View.OnTouchListener, View.OnGenericMotionListener {
    protected LibraryManager wineActivity;
    protected XServerManager xserver;
    protected Mouse mouse;

    public final static int INPUT_TOUCH_SCREEN = 0;
    public final static int INPUT_MOUSE = 1;

    protected boolean isMoving = false;
    protected int inputDevice = INPUT_TOUCH_SCREEN;

    protected PointF point1;

    public abstract void updateWindow(MotionEvent event);

    public abstract void stopMoving();

    protected abstract boolean onTouchTouchScreen(MotionEvent event);

    protected boolean onTouchMouse(MotionEvent event) {
        int action = event.getActionMasked();
        point1 = new PointF(event.getX(), event.getY());
        PointF point = xserver.getDesktopView().getDesktopCords(event.getX(), event.getY());
        try {
            point = xserver.getFocusedWindow().convertDeskCordsToWin(point.x, point.y);
            event.setLocation(point.x, point.y);
        } catch (ClassNotFoundException e) {
            return true;
        }
        switch (action) {
            case MotionEvent.ACTION_DOWN:
                MouseActions.setButtonClick(event.getX(), event.getY(), xserver.getWM(), MouseActions.MOUSE_DOWN, MouseActions.LEFT_BUTTON);
                break;
            case MotionEvent.ACTION_MOVE:
                if (xserver.getFocusedWindow().getCanMove()) {
                    if (!isMoving) {
                        isMoving = true;
                    }
                    try {
                        mouse.setPosition(xserver.getFocusedWindow().convertWinCordsToDesk(event.getX(), event.getY()));
                    } catch (ClassNotFoundException e) {}
                    MouseActions.setButtonClick(event.getX(), event.getY(), xserver.getWM(), MouseActions.MOUSE_MOVE, MouseActions.LEFT_BUTTON);
                }
                break;
            case MotionEvent.ACTION_UP:
                if (isMoving) {
                    isMoving = false;
                }
                MouseActions.setButtonClick(event.getX(), event.getY(), xserver.getWM(), MouseActions.MOUSE_UP, MouseActions.LEFT_BUTTON);
                xserver.getFocusedWindow().setCanMove(true);
                break;
        }
        return true;
    }

    protected boolean onMoveMouse(MotionEvent event) {
        point1 = new PointF(event.getX(), event.getY());
        PointF point = xserver.getDesktopView().getDesktopCords(event.getX(), event.getY());
        try {
            point = xserver.getFocusedWindow().convertDeskCordsToWin(point.x, point.y);
            event.setLocation(point.x, point.y);
        } catch (ClassNotFoundException e) {
            return true;
        }
        try {
            mouse.setPosition(xserver.getFocusedWindow().convertWinCordsToDesk(event.getX(), event.getY()));
        } catch (ClassNotFoundException e) {}
        MouseActions.setButtonClick(event.getX(), event.getY(), xserver.getWM(), MouseActions.MOUSE_MOVE, MouseActions.LEFT_BUTTON);
        if (event.getAction() == MotionEvent.ACTION_SCROLL) {
            if (event.getAxisValue(MotionEvent.AXIS_VSCROLL) < 0.0f) {
                MouseActions.scroll(mouse.getX(), mouse.getY(), xserver.getWM(), -1);
            } else {
                MouseActions.scroll(mouse.getX(), mouse.getY(), xserver.getWM(), 1);
            }
        }
        return false;
    }

    @Override
    public boolean onGenericMotion(View v, MotionEvent event) {
        if (event.getToolType(0) == MotionEvent.TOOL_TYPE_MOUSE) {
            return onMoveMouse(event);
        }
        return false;
    }
}
