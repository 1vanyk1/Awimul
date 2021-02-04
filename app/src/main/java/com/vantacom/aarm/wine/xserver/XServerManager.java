package com.vantacom.aarm.wine.xserver;

import android.content.Context;
import android.graphics.Point;
import android.graphics.PointF;
import android.graphics.Rect;
import android.util.Log;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.views.DesktopView;
import com.vantacom.aarm.wine.WineActivity;
import com.vantacom.aarm.wine.controls.Keyboard;
import com.vantacom.aarm.wine.views.Window;
import com.vantacom.aarm.wine.views.WindowsGroup;

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

    public XServerManager(int screenWidth, int screenHeight, int desktopWidth, int desktopHeight, WineActivity activity, CustomClassManager wineActivity, Keyboard keyboard) {
        this.screenWidth = screenWidth;
        this.screenHeight = screenHeight;
        this.desktopWidth = desktopWidth;
        this.desktopHeight = desktopHeight;
        this.activity = activity;
        this.wineActivity = wineActivity;
        this.keyboard = keyboard;
        zOrder = new ArrayList<Integer>();
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

    public boolean isSystemPaused() {
        return activity.isSystemPaused();
    }

    public PointF convertWinCordsToDesk(PointF p, Window w) throws ClassNotFoundException {
        return convertWinCordsToDesk(p.x, p.y,  w);
    }

    public PointF convertWinCordsToDesk(float x, float y, Window w) throws ClassNotFoundException {
        if (w.getGroup(false) == null) {
            throw new ClassNotFoundException();
        }
        float left = w.getGroup(false).getLeft();
        float top = w.getGroup(false).getTop();
        return new PointF(x + left, y + top);
    }

    public PointF convertDeskCordsToWin(PointF p, Window w) throws ClassNotFoundException{
        return convertDeskCordsToWin(p.x, p.y,  w);
    }

    public PointF convertDeskCordsToWin(float x, float y, Window w) throws ClassNotFoundException {
        if (w.getGroup(false) == null) {
            throw new ClassNotFoundException();
        }
        float left = w.getGroup(false).getLeft();
        float top = w.getGroup(false).getTop();
        return new PointF(x - left, y - top);
    }

    public boolean getPointTouchWindowDeskCords(PointF p, Window window) {
        float x = p.x;
        float y = p.y;
        WindowsGroup wg = window.getGroup(false);
        return (wg.getLeft() <= x && wg.getRight() >= x && wg.getTop() <= y && wg.getBottom() >= y);
    }

    public void f2(float x, float y, Window w) {
        PointF point;
        try {
            point = convertWinCordsToDesk(x, y, w);
        } catch (ClassNotFoundException e) {
            return;
        }
        Window window;
        PointF point2;
        for (int i = 0; i < zOrder.size(); i++) {
            if (zOrder.get(i) == desktopView.getDesktopWindow().getHWND()) {
                continue;
            }
            window = getWindowByZOrder(i);
            if (window.getParent() == null || window.getParent() == desktopView.getDesktopWindow()) {
                if (getPointTouchWindowDeskCords(point, window)) {
                    try {
                        point2 = convertDeskCordsToWin(point, window);
                    } catch (ClassNotFoundException e) {
                        continue;
                    }
                    Log.e(String.valueOf(window.getHWND()), String.format("%f %f", point2.x, point2.y));
                    return;
                }
            }
        }
        window = desktopView.getDesktopWindow();
        try {
            point2 = convertDeskCordsToWin(point, window);
        } catch (ClassNotFoundException e) {
            return;
        }
        Log.e(String.valueOf(window.getHWND()), String.format("%f %f", point2.x, point2.y));
//        point2 = desktopView.getDesktopCords();
//        Log.e("Desktop", String.format("%f %f", point2.x, point2.y));
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
        for (int i = 0; i < zOrder.size(); i++) {
            s.append(zOrder.get(i)).append(" ");
        }
        Log.e("f", s.toString());
    }

    public void createDesktopWindow(int hwnd) {
        this.desktopView = new DesktopView(this, wineActivity, activity, hwnd, desktopWidth, desktopHeight, screenWidth, screenHeight);
        addWindow(hwnd, desktopView.getDesktopWindow());
        Log.e("f", String.valueOf(hwnd));
//        zOrder.add(hwnd);
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
//            zOrder.add(hwnd);
            window = new Window(this, hwnd, getWindow(parent), scale);
            addWindow(hwnd, window);
            window.createWindowGroups();
            if (window.getParent() == desktopView.getDesktopWindow()) {
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
            removeWindow(window);
            zOrder.remove((Object)hwnd);
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
        keyboard = null;
    }
}
