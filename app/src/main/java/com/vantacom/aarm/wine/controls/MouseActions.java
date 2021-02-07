package com.vantacom.aarm.wine.controls;

import android.view.MotionEvent;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.xserver.views.Window;

public class MouseActions {
    public static final int LEFT_BUTTON = 0;
    public static final int RIGHT_BUTTON = 2;

    public static final int MOUSE_DOWN = 0;
    public static final int MOUSE_UP = 1;
    public static final int MOUSE_MOVE = 2;
    public static final int MOUSE_SCROLL = 8;

    public static boolean singleLeftButtonClick(MotionEvent event, CustomClassManager activity, Window window) {
        int[] eventPos = window.getEventPos(event);
        activity.invoke("wine_motion_event", window.getHWND(), MOUSE_DOWN, eventPos[0], eventPos[1], LEFT_BUTTON, 0);
        return (boolean)activity.invoke("wine_motion_event", window.getHWND(), MOUSE_UP, eventPos[0], eventPos[1], LEFT_BUTTON, 0);
    }

    public static void singleRightButtonClick(MotionEvent event, CustomClassManager activity, Window window) {
        if (window.getGroup(false) != null ) {
            int[] eventPos = window.getEventPos(event);
            activity.invoke("wine_motion_event", window.getHWND(), MOUSE_DOWN, eventPos[0], eventPos[1], RIGHT_BUTTON, 0);
            activity.invoke("wine_motion_event", window.getHWND(), MOUSE_UP, eventPos[0], eventPos[1], RIGHT_BUTTON, 0);
            activity.invoke("wine_motion_event", window.getHWND(), MOUSE_UP, eventPos[0], eventPos[1], LEFT_BUTTON, 0);
        }
    }

    public static boolean setLeftButtonClick(MotionEvent event, CustomClassManager activity, Window window, int action) {
        int[] eventPos = window.getEventPos(event);
        return (boolean)activity.invoke("wine_motion_event", window.getHWND(), action, eventPos[0], eventPos[1], LEFT_BUTTON, 0);
    }

    public static void scroll(MotionEvent event, CustomClassManager activity, Window window, int vscroll) {
        int[] eventPos = window.getEventPos(event);
        activity.invoke("wine_motion_event", window.getHWND(), MOUSE_SCROLL, eventPos[0], eventPos[1], 0, vscroll);
    }
}
