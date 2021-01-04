package com.vantacom.aarm.wine.controls;

import android.view.MotionEvent;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.views.Window;

//    event == 0  on down
//    event == 1  on up
//    event == 2  on move

//    buttonState == 0  left button
//    buttonState == 2  right button

public class MouseActions {
    public static boolean singleLeftButtonClick(MotionEvent event, CustomClassManager activity, Window window) {
        int[] eventPos = window.getEventPos(event);
        activity.invoke("wine_motion_event", window.getHWND(), 0, eventPos[0], eventPos[1], 1, 0);
        return (boolean)activity.invoke("wine_motion_event", window.getHWND(), 1, eventPos[0], eventPos[1], 1, 0);
    }

    public static void singleRightButtonClick(MotionEvent event, CustomClassManager activity, Window window) {
        if (window.getGroup(false) != null ) {
            int[] eventPos = window.getEventPos(event);
            activity.invoke("wine_motion_event", window.getHWND(), 0, eventPos[0], eventPos[1], 2, 0);
            activity.invoke("wine_motion_event", window.getHWND(), 1, eventPos[0], eventPos[1], 2, 0);
        }
    }

    public static boolean setLeftButtonClick(MotionEvent event, CustomClassManager activity, Window window, int action) {
        int[] eventPos = window.getEventPos(event);
        return (boolean)activity.invoke("wine_motion_event", window.getHWND(), action, eventPos[0], eventPos[1], 0, 0);
    }
}
