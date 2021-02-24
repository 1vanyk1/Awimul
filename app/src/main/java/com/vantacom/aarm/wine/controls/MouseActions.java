package com.vantacom.aarm.wine.controls;

import com.vantacom.aarm.LibraryManager;
import com.vantacom.aarm.wine.xserver.views.Window;

public class MouseActions {
    public static final int LEFT_BUTTON = 0;
    public static final int RIGHT_BUTTON = 2;

    public static final int MOUSE_DOWN = 0;
    public static final int MOUSE_UP = 1;
    public static final int MOUSE_MOVE = 2;
    public static final int MOUSE_SCROLL = 8;

    public static boolean singleLeftButtonClick(float x, float y, LibraryManager activity, Window window) {
        int[] eventPos = window.getEventPos(x, y);
        activity.invoke("wine_motion_event", window.getHWND(), MOUSE_DOWN, eventPos[0], eventPos[1], LEFT_BUTTON, 0);
        return (boolean)activity.invoke("wine_motion_event", window.getHWND(), MOUSE_UP, eventPos[0], eventPos[1], LEFT_BUTTON, 0);
    }

    public static void singleRightButtonClick(float x, float y, LibraryManager activity, Window window) {
        if (window.getGroup(false) != null ) {
            int[] eventPos = window.getEventPos(x, y);
            activity.invoke("wine_motion_event", window.getHWND(), MOUSE_DOWN, eventPos[0], eventPos[1], RIGHT_BUTTON, 0);
            activity.invoke("wine_motion_event", window.getHWND(), MOUSE_UP, eventPos[0], eventPos[1], RIGHT_BUTTON, 0);
            activity.invoke("wine_motion_event", window.getHWND(), MOUSE_UP, eventPos[0], eventPos[1], LEFT_BUTTON, 0);
        }
    }

    public static boolean setLeftButtonClick(float x, float y, LibraryManager activity, Window window, int action) {
        int[] eventPos = window.getEventPos(x, y);
        return (boolean)activity.invoke("wine_motion_event", window.getHWND(), action, eventPos[0], eventPos[1], LEFT_BUTTON, 0);
    }

    public static void scroll(float x, float y, LibraryManager activity, Window window, int vscroll) {
        int[] eventPos = window.getEventPos(x, y);
        activity.invoke("wine_motion_event", window.getHWND(), MOUSE_SCROLL, eventPos[0], eventPos[1], 0, vscroll);
    }
}
