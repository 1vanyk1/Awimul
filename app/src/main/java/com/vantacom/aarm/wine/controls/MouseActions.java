package com.vantacom.aarm.wine.controls;

import com.vantacom.aarm.LibraryManager;
import com.vantacom.aarm.wine.xserver.views.Window;
import com.vantacom.aarm.xserver.WM;

public class MouseActions {
    public static final int LEFT_BUTTON = 1;
    public static final int RIGHT_BUTTON = 3;

    public static final int MOUSE_DOWN = 1;
    public static final int MOUSE_UP = 0;
    public static final int MOUSE_MOVE = 2;
    public static final int MOUSE_SCROLL = 8;

    public static boolean singleLeftButtonClick(float x, float y, WM wm) {
        wm.mouseEvent(MOUSE_DOWN, (int)x, (int)y, LEFT_BUTTON);
        wm.mouseEvent(MOUSE_UP, (int)x, (int)y, LEFT_BUTTON);
        return true;
    }

    public static void singleRightButtonClick(float x, float y, WM wm) {
        wm.mouseEvent(MOUSE_DOWN, (int)x, (int)y, RIGHT_BUTTON);
        wm.mouseEvent(MOUSE_UP, (int)x, (int)y, RIGHT_BUTTON);
    }

    public static boolean setLeftButtonClick(float x, float y, WM wm, int action) {
        wm.mouseEvent(action, (int)x, (int)y, LEFT_BUTTON);
        return true;
    }

    public static boolean setButtonClick(float x, float y, WM wm, int action, int button) {
        wm.mouseEvent(action, (int)x, (int)y, button);
        return true;
//        return (boolean)activity.invoke("wine_motion_event", window.getHWND(), action, eventPos[0], eventPos[1], button, 0);
    }

    public static void scroll(float x, float y, WM wm, int vscroll) {
//        int[] eventPos = window.getEventPos(x, y);
//        activity.invoke("wine_motion_event", window.getHWND(), MOUSE_SCROLL, eventPos[0], eventPos[1], 0, vscroll);
    }
}
