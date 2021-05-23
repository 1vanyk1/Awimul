package com.vantacom.aarm.wine.xserver;

import android.content.Context;
import android.graphics.PointF;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.util.Log;

import com.vantacom.aarm.LibraryManager;
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
    private LibraryManager wineActivity;
    private Keyboard keyboard;
    private DesktopView desktopView;
    private ArrayList<Integer> zOrder;
    private ResizeManager resizeManager;
    private Window focusedWindow;
    private Mouse cursor;

    private final static int LOG_PIXELS = 96;

    public XServerManager(int screenWidth, int screenHeight, int desktopWidth, int desktopHeight, WineActivity activity, LibraryManager wineActivity) {
        this.screenWidth = screenWidth;
        this.screenHeight = screenHeight;
        this.desktopWidth = desktopWidth;
        this.desktopHeight = desktopHeight;
        this.activity = activity;
        this.wineActivity = wineActivity;
        this.keyboard = new Keyboard(activity, this);
        zOrder = new ArrayList<Integer>();
        resizeManager = new ResizeManager(this);
        cursor = new Mouse(this, desktopWidth / 2, desktopHeight / 2);
    }

    public void updateActivity(WineActivity activity, boolean wineLoaded) {
        Window window;
        this.activity = activity;
        this.keyboard = new Keyboard(activity, this);
        if (wineLoaded) {
            int hwndFocus = focusedWindow.getHWND();
            SurfaceTexture clientSurfTex = desktopView.getDesktopWindow().getClientSurfTex();
            SurfaceTexture windowSurfTex = desktopView.getDesktopWindow().getWindowSurfTex();
            int desktopHWND = desktopView.getDesktopWindow().getHWND();
            window = desktopView.getDesktopWindow();
            int parent = -3;
            int vis = window.getVis();
            int next_hwnd = window.getNextHWND();
            int owner = window.getOwnerHWND();
            int style = window.getStyle();
            Rect clientRect = window.getClientRect();
            Rect windowRect = window.getWindowRect();
            window.disconnectSurface();
            desktopView.disconnectCursor();
            this.desktopView = new DesktopView(this, activity, desktopHWND, desktopWidth, desktopHeight, screenWidth, screenHeight);
            window = desktopView.getDesktopWindow();
            window.createWindowGroups();
            window.createWindowView();
            window.createClientView();
            if (zOrder.contains(desktopHWND)) {
                if (clientSurfTex != null) {
                    window.setSurface(clientSurfTex, true);
                }
                if (windowSurfTex != null) {
                    window.setSurface(windowSurfTex, false);
                }
                window.posChanged(vis, next_hwnd, owner, style, clientRect, windowRect, false);
            }
            addWindow(desktopHWND, window);
            for (int i = 0; i < zOrder.size(); i++) {
                window = getWindow(zOrder.get(i));
                if (window != null && window.getHWND() != desktopView.getDesktopWindow().getHWND()) {
                    int hwnd = window.getHWND();
                    parent = -3;
                    vis = window.getVis();
                    next_hwnd = window.getNextHWND();
                    owner = window.getOwnerHWND();
                    style = window.getStyle();
                    clientSurfTex = window.getClientSurfTex();
                    windowSurfTex = window.getWindowSurfTex();
                    clientRect = window.getClientRect();
                    windowRect = window.getWindowRect();
                    if (window.getParent() != null) {
                        parent = window.getParent().getHWND();
                    }
                    float scale = window.getScale();
                    window.disconnectSurface();
                    window = new Window(this, hwnd, getWindow(parent), scale);
                    addWindow(zOrder.get(i), window);
                    if (parent >= 0) {
                        window.setParent(getWindow(parent), scale);
                    }
                    window.createWindowGroups();
                    if (window.getParent() == desktopView.getDesktopWindow()) {
                        window.createWindowView();
                    }
                    window.createClientView();
                    if (clientSurfTex != null) {
                        window.setSurface(clientSurfTex, true);
                    }
                    if (windowSurfTex != null) {
                        window.setSurface(windowSurfTex, false);
                    }
                    window.posChanged(vis, next_hwnd, owner, style, clientRect, windowRect, false);
                }
            }
            changeFocus(windowsHM.get(hwndFocus));
            syncViewsZOrder();
        }
    }

    public int getDesktopWidth() { return desktopWidth; }

    public int getDesktopHeight() { return desktopHeight; }

    public Context getContext() { return activity; }

    public LibraryManager getWineActivity() { return wineActivity; }

    public Keyboard getKeyboard() { return keyboard; }

    public Mouse getCursor() { return cursor; }

    public DesktopView getDesktopView() { return desktopView; }

    public Window getWindow(int hwnd) { return windowsHM.get(hwnd); }

    private Window getWindowByZOrder(int i) { return windowsHM.get(zOrder.get(i)); }

    public int getZOrderByWindow(Window window) { return zOrder.indexOf(window.getHWND()); }

    private void addWindow(int hwnd, Window window) { windowsHM.put(hwnd, window); }

    private void removeWindow(Window window) {
        try {
            windowsHM.remove(window);
        } catch (Exception e) {
            Log.e("removeWindow", String.format("Can't remove window %d", window.getHWND()));
        }
    }

    public void changeZOrder(int hwnd, int prev_hwnd, boolean isFirst) {
        zOrder.remove((Object)hwnd);
        int index = 0;
        if (prev_hwnd != 0) {
            for (int i = 0; i < zOrder.size(); i++) {
                if (getWindowByZOrder(i).getNextHWND() < 0) {
                    index++;
                }
                if (zOrder.get(i) == prev_hwnd) {
                    index = i + 1;
                    break;
                }
            }
        }
        zOrder.add(index, hwnd);
        Window window = windowsHM.get(hwnd);
        for (int j = 0; j < window.getCountOfViews(); j++) {
            if (j == 0) {
                changeZOrder(window.getView(j).getHWND(), hwnd, false);
            } else {
                changeZOrder(window.getView(j).getHWND(), window.getView(j - 1).getHWND(), false);
            }
        }
        if (isFirst) {
            syncViewsZOrder();
        }
    }

    public void syncViewsZOrder() {
        Window window;
        for (int i = zOrder.size() - 1; i >= 0; i--) {
            window = getWindowByZOrder(i);
            if (window.getParent() != null && window.getParent() == getDesktopView().getDesktopWindow()) {
                window.getGroup(false).bringToFront();
            }
        }
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

    private Window getTouchedWindow(PointF point) {
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

    public Window getTouchedWindow(float x, float y) {
        return getTouchedWindow(desktopView.getDesktopCords(x, y));
    }

    public Window getTouchedWindow(Mouse mouse) {
        return getTouchedWindow(new PointF(mouse.getX(), mouse.getY()));
    }

    public Window getFocusedWindow() {
        return focusedWindow;
    }

    public void moveToFront(Window window) {
        window.setZOrder(null, true);
        for (int i = window.getCountOfOwnedWindow() - 1; i >= 0; i--) {
            moveToFront(window.getOwnedWindow(i));
        }
    }

    public void changeFocus(Window w) {
        if (focusedWindow != w) {
            if (getDesktopView().getDesktopWindow() == w) {
                focusedWindow = w;
            } else if (w.getParent() == getDesktopView().getDesktopWindow()) {
                focusedWindow = w;
                Window window = focusedWindow;
                while (window.getOwnerHWND() != 0) {
                    window = window.getOwner();
                }
                moveToFront(window);
            }
        }
    }

    public void createDesktopWindow(int hwnd) {
        if (activity == null) {
            this.desktopView = new DesktopView(this, activity, hwnd, desktopWidth, desktopHeight, screenWidth, screenHeight);
            addWindow(hwnd, desktopView.getDesktopWindow());
            changeFocus(desktopView.getDesktopWindow());
            try {
                wineActivity.invoke("wine_config_changed", LOG_PIXELS);
            } catch (Exception e) {
                Log.e("wine", e.toString());
            }
        } else {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    XServerManager.this.desktopView = new DesktopView(XServerManager.this, activity, hwnd, desktopWidth, desktopHeight, screenWidth, screenHeight);
                    addWindow(hwnd, desktopView.getDesktopWindow());
                    changeFocus(desktopView.getDesktopWindow());
                    try {
                        wineActivity.invoke("wine_config_changed", LOG_PIXELS);
                    } catch (Exception e) {
                        Log.e("wine", e.toString());
                    }
                }
            });
            activity.createDesktopWindow(XServerManager.this);
            wineActivity.invoke("wine_desktop_changed", desktopWidth, desktopHeight);
        }
    }

    public void createWindow(int hwnd, boolean isClient, int parent, float scale) {
        if (activity == null) {
            Window window = getWindow(hwnd);
            if (!zOrder.contains(hwnd)) {
                zOrder.add(hwnd);
            }
            if (window == null) {
                window = new Window(this, hwnd, getWindow(parent), scale);
                addWindow(hwnd, window);
                window.createWindowGroups();
                if (window.getParent() == desktopView.getDesktopWindow()) {
                    changeFocus(window);
                    window.createWindowView();
                }
            }
            if (isClient) {
                window.createClientView();
            }
        } else {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Window window = getWindow(hwnd);
                    if (!zOrder.contains(hwnd)) {
                        zOrder.add(hwnd);
                    }
                    if (window == null) {
                        window = new Window(XServerManager.this, hwnd, getWindow(parent), scale);
                        addWindow(hwnd, window);
                        window.createWindowGroups();
                        if (window.getParent() == desktopView.getDesktopWindow()) {
                            changeFocus(window);
                            window.createWindowView();
                        }
                    }
                    if (isClient) {
                        window.createClientView();
                    }
                }
            });
        }
    }

    public void destroyWindow(int hwnd) {
        if (activity == null) {
            Window window = getWindow(hwnd);
            if (window != null) {
                if (focusedWindow == window) {
                    changeFocus(getDesktopView().getDesktopWindow());
                }
                removeWindow(window);
                if (zOrder.contains(hwnd)) {
                    zOrder.remove((Object) hwnd);
                }
                window.destroy();
            }
        } else {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Window window = getWindow(hwnd);
                    if (window != null) {
                        if (focusedWindow == window) {
                            changeFocus(getDesktopView().getDesktopWindow());
                        }
                        removeWindow(window);
                        if (zOrder.contains(hwnd)) {
                            zOrder.remove((Object) hwnd);
                        }
                        window.destroy();
                    }
                }
            });
        }
    }

    public void setWindowParent(int hwnd, int hwnd_parent, float scale) {
        if (activity == null) {
            Window window = getWindow(hwnd);
            if (window != null) {
                window.setParent(getWindow(hwnd_parent), scale);
                if (window.getParent() == desktopView.getDesktopWindow()) {
                    window.createWindowView();
                }
            }
        } else {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Window window = getWindow(hwnd);
                    if (window != null) {
                        window.setParent(getWindow(hwnd_parent), scale);
                        if (window.getParent() == desktopView.getDesktopWindow()) {
                            window.createWindowView();
                        }
                    }
                }
            });
        }

    }

    public void windowPosChanged(int hwnd, int vis, int next_hwnd, int owner, int style, Rect win_rect, Rect client_rect, Rect visible_rect) {
        if (activity == null) {
            Window window = getWindow(hwnd);
            if (window != null) {
                window.posChanged(vis, next_hwnd, owner, style, client_rect, visible_rect, true);
            }
        } else {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Window window = getWindow(hwnd);
                    if (window != null) {
                        window.posChanged(vis, next_hwnd, owner, style, client_rect, visible_rect, true);
                    }
                }
            });
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
