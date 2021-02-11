package com.vantacom.aarm.wine.xserver;

import android.content.Context;
import android.graphics.PointF;
import android.graphics.Rect;
import android.util.Log;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.xserver.views.DesktopView;
import com.vantacom.aarm.wine.WineActivity;
import com.vantacom.aarm.wine.controls.keyboard.Keyboard;
import com.vantacom.aarm.wine.xserver.views.Window;
import com.vantacom.aarm.wine.xserver.views.WindowsGroup;

import java.util.ArrayList;
import java.util.HashMap;

public class XServerManager {
    private HashMap<Integer, Window> windowsHM = new HashMap<Integer, Window>();
    private int screenWidth, screenHeight;
    private int desktopWidth, desktopHeight;
    private WineActivity activity;
    private CustomClassManager wineActivity;
    private Keyboard keyboard;
    private DesktopView desktopView;
    private ArrayList<Integer> zOrder;
    private ResizeManager resizeManager;
    private Window focusedWindow;

    public XServerManager(int screenWidth, int screenHeight, int desktopWidth, int desktopHeight, WineActivity activity, CustomClassManager wineActivity) {
        this.screenWidth = screenWidth;
        this.screenHeight = screenHeight;
        this.desktopWidth = desktopWidth;
        this.desktopHeight = desktopHeight;
        this.activity = activity;
        this.wineActivity = wineActivity;
        this.keyboard = new Keyboard(activity, this);
        zOrder = new ArrayList<Integer>();
        resizeManager = new ResizeManager(this);
    }

    public Context getContext() { return activity; }

    public CustomClassManager getWineActivity() { return wineActivity; }

    public Keyboard getKeyboard() { return keyboard; }

    public DesktopView getDesktopView() { return desktopView; }

    public Window getWindow(int hwnd) { return windowsHM.get(hwnd); }

    private Window getWindowByZOrder(int i) { return windowsHM.get(zOrder.get(i)); }

    private void addWindow(int hwnd, Window window) { windowsHM.put(hwnd, window); }

    private void removeWindow(Window window) {
        try {
            windowsHM.remove(window);
        } catch (Exception e) {
            Log.e("removeWindow", String.format("Can't remove window %d", window.getHWND()));
        }
    }

    public void changeZOrder(int hwnd, int prev_hwnd) {
        zOrder.remove((Object)hwnd);
        for (int i = 0; i < zOrder.size(); i++) {
            if (zOrder.get(i) == prev_hwnd) {
                zOrder.add(i + 1, hwnd);
                Window window = windowsHM.get(hwnd);
                for (int j = 0; j < window.getCountOFViews(); j++) {
                    if (j == 0) {
                        changeZOrder(window.getView(j).getHWND(), hwnd);
                    } else {
                        changeZOrder(window.getView(j).getHWND(), window.getView(j - 1).getHWND());
                    }
                }
                break;
            }
        }
    }

    public void syncViewsZOrder() {
        Window window;
        for (int i = zOrder.size() - 1; i >= 0; i--) {
            window = getWindowByZOrder(i);
            if (window.getParent() == null || window.getParent() == getDesktopView().getDesktopWindow()) {
                window.getGroup(false).bringToFront();
            }
        }
    }

    public void moveToTopZOrder(int hwnd) {
        Window window = windowsHM.get(hwnd);
        if (window.getParent() == getDesktopView().getDesktopWindow()) {
            window.setZOrder(null);
        }

//        zOrder.remove((Object)hwnd);
//        zOrder.add(0, hwnd);
//        Window window = windowsHM.get(hwnd);
//        for (int j = 0; j < window.getCountOFViews(); j++) {
//            if (j == 0) {
//                changeZOrder(window.getView(j).getHWND(), hwnd);
//            } else {
//                changeZOrder(window.getView(j).getHWND(), window.getView(j - 1).getHWND());
//            }
//        }
    }

    public boolean isSystemPaused() {
        return activity.isSystemPaused();
    }

    public boolean getPointTouchWindowDeskCords(PointF p, Window window) {
        float x = p.x;
        float y = p.y;
        WindowsGroup wg = window.getGroup(false);
        return (wg.getLeft() <= x && wg.getRight() >= x && wg.getTop() <= y && wg.getBottom() >= y);
    }

    public Window getTouchedWindow(float x, float y) {
        PointF point = desktopView.getDesktopCords(x, y);
        Window window;
        for (int i = 0; i < zOrder.size(); i++) {
            if (zOrder.get(i) == desktopView.getDesktopWindow().getHWND()) {
                continue;
            }
            window = getWindowByZOrder(i);
            if (window.getParent() == null || window.getParent() == desktopView.getDesktopWindow()) {
                if (getPointTouchWindowDeskCords(point, window)) {
                    return window;
                }
            }
        }
        return desktopView.getDesktopWindow();
    }

    public void f() {
        StringBuilder s = new StringBuilder();
        Window window;
        for (int i = 0; i < zOrder.size(); i++) {
            window = getWindowByZOrder(i);
            if (window.getParent() == getDesktopView().getDesktopWindow()) {
                s.append(zOrder.get(i)).append(" ");
            }

        }
        Log.e("f", s.toString());
    }

    public Window getFocusedWindow() {
        return focusedWindow;
    }

    public void changeFocus(Window w) {
        if (focusedWindow != w && w.getParent() == getDesktopView().getDesktopWindow()) {
            focusedWindow = w;
            moveToTopZOrder(getFocusedWindow().getHWND());
            syncViewsZOrder();
        }
    }

    public void createDesktopWindow(int hwnd) {
        this.desktopView = new DesktopView(this, wineActivity, activity, hwnd, desktopWidth, desktopHeight, screenWidth, screenHeight);
        addWindow(hwnd, desktopView.getDesktopWindow());
        focusedWindow = desktopView.getDesktopWindow();
        try {
            wineActivity.invoke("wine_config_changed", activity.getResources().getConfiguration().densityDpi);
        } catch (Exception e) {
            Log.e("wine", e.toString());
        }
    }

    public void createWindow(int hwnd, boolean isClient, int parent, float scale) {
        Window window = getWindow(hwnd);
        if (!zOrder.contains(hwnd)) {
            zOrder.add(hwnd);
        }
        if (window == null) {
            window = new Window(this, hwnd, getWindow(parent), scale);
            addWindow(hwnd, window);
            window.createWindowGroups();
            if (window.getParent() == desktopView.getDesktopWindow()) {
                focusedWindow = window;
                window.createWindowView();
            }
        }
        if (isClient) {
            window.createClientView();
        }
    }

    public void destroyWindow(int hwnd) {
        Window window = getWindow(hwnd);
        if (window != null) {
            if (focusedWindow == window) {
                focusedWindow = getDesktopView().getDesktopWindow();
            }
            removeWindow(window);
            if (zOrder.contains(hwnd)) {
                zOrder.remove((Object)hwnd);
            }
            window.destroy();
        }
    }

    public void setWindowsParent(int hwnd, int hwnd_parent, float scale) {
        Window window = getWindow(hwnd);
        if (window != null) {
            window.setParent(getWindow(hwnd_parent), scale);
            if (window.getParent() == desktopView.getDesktopWindow()) {
                window.createWindowView();
            }
        }
    }

    public void windowPosChanged(int hwnd, int vis, int next_hwnd, int owner, int style, Rect win_rect, Rect client_rect, Rect visible_rect) {
        Window window = getWindow(hwnd);
        if (window != null) {
            window.posChanged(vis, next_hwnd, owner, style, client_rect, visible_rect);
        }
    }

    public void destroy() {
        Window[] windowsArray = windowsHM.values().toArray(new Window[0]);
        for (int i = 0; i < windowsHM.values().size(); i++) {
            removeWindow(windowsArray[i]);
            windowsArray[i].destroy();
        }
        windowsHM.clear();
        removeWindow(desktopView.getDesktopWindow());
        desktopView.destroy();
        activity = null;
        desktopView = null;
        wineActivity = null;
        keyboard.destroy();
        keyboard = null;
    }

    public float getScale() {
        return resizeManager.getScale();
    }

    public ResizeManager getResizeManager() {
        return resizeManager;
    }

    public void toggleTopBar() {
        activity.toggleTopBar();
    }
}
