package com.vantacom.aarm.wine.views;

import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.View;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.WineActivity;

import java.util.ArrayList;

public class Window {
    private ArrayList<Window> children;
    private int hwnd;
    private boolean visible;
    private float scale;
    private Window parent;
    private Rect windowRect, clientRect;
    private WineActivity activity;
    private CustomClassManager wineActivity;
    private Surface windowSurface, clientSurface;
    private WindowsGroup windowGroup, clientGroup;
    private SurfaceTexture windowSurfTex, clientSurfTex;

    public Window(WineActivity activity, CustomClassManager wineActivity, int hwnd, Window parent, float scale) {
        this.activity = activity;
        this.wineActivity = wineActivity;
        this.hwnd = hwnd;
        this.visible = false;
        Rect rect = new Rect(0, 0, 0, 0);
        this.clientRect = rect;
        this.windowRect = rect;
        this.parent = parent;
        this.scale = scale;
        this.children = new ArrayList<Window>();
        this.activity.addWindow(hwnd, this);
        if (parent != null) {
            parent.addView(this);
        }
    }

    public int getHWND() { return hwnd; }

    public float getScale() {
        return scale;
    }

    public Window getParent() { return parent; }

    public void addView(Window window) {
        children.add(window);
    }

    public void addView(int index, Window window) {
        children.add(index, window);
    }

    public void removeView(Window window) {
        children.remove(window);
    }

    public int getIndexOFView(Window window) {
        return children.indexOf(window);
    }

    public int getCountOFViews() {
        return children.size();
    }

    public Window getView(int index) { return children.get(index); }

    protected void removeViewFromParent() { parent.clientGroup.removeView(windowGroup); }

    public WindowsGroup getGroup(boolean isClient) {
        if (isClient) { return clientGroup; }
        return windowGroup;
    }

    public void createClientView() {
        if (clientGroup == null) { createWindowGroups(); }
        clientGroup.createContentView(activity, wineActivity, true).layout(0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
    }

    public View createWindowView() {
        if (windowGroup == null) { createWindowGroups(); }
        windowGroup.createContentView(activity, wineActivity, false).layout(0, 0, Math.round((windowRect.right - windowRect.left) * scale), Math.round((windowRect.bottom - windowRect.top) * scale));
        windowGroup.setScale(scale);
        return windowGroup;
    }

    public void destroy() {
        visible = false;
        activity.removeWindow(this);
        if (parent != null) {
            parent.removeView(this);
        }
        if (windowGroup != null) {
            if (parent != null && parent.clientGroup != null) {
                removeViewFromParent();
            }
            windowGroup.destroyContentView();
            windowGroup = null;
        }
        if (clientGroup != null) {
            clientGroup.destroyContentView();
            clientGroup = null;
        }
        if (windowSurfTex != null) {
            windowSurfTex.release();
            windowSurfTex = null;
        }
        if (clientSurfTex != null) {
            clientSurfTex.release();
            clientSurfTex = null;
        }
        if (windowSurface != null) {
            windowSurface.release();
            windowSurface = null;
        }
        if (clientSurface != null) {
            clientSurface.release();
            clientSurface = null;
        }
    }

    public void createWindowGroups() {
        if (clientGroup == null) {
            windowGroup = new WindowsGroup(activity, this);
            clientGroup = new WindowsGroup(activity, this);
            windowGroup.addView(clientGroup);
            clientGroup.setLayout(clientRect.left - windowRect.left, clientRect.top - windowRect.top, clientRect.right - windowRect.left, clientRect.bottom - windowRect.top);
            if (parent != null) {
                parent.createWindowGroups();
                if (visible) {
                    addViewToParent();
                }
                windowGroup.setLayout(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);
            }
        }
    }

    protected void addViewToParent() {
        int pos = parent.clientGroup.getChildCount() - 1;
        if (pos >= 0 && parent.clientGroup.getChildAt(pos) == parent.clientGroup.getContentView()) {
            pos -= 1;
        }
        Window window;
        for (int i = 0; i < parent.getCountOFViews() && pos >= 0; i++) {
            window = parent.getView(i);
            if (window == this) { break; }
            if (window.visible && window == ((WindowsGroup)parent.clientGroup.getChildAt(pos)).getWindow()) {
                pos -= 1;
            }
        }
        parent.clientGroup.addView(windowGroup, pos + 1);
    }

    public void posChanged(int vis, int next_hwnd, int owner, int style, Rect clientRect, Rect windowRect)
    {
        boolean visible = this.visible;
        this.windowRect = windowRect;
        this.clientRect = clientRect;
        style = style & 0x10000000;
        this.visible = style != 0;
        if ((vis & View.INVISIBLE) == 0 && parent != null) {
            setZOrder(activity.getWindow(next_hwnd));
        }
        if (windowGroup != null) {
            windowGroup.setLayout(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);
            if (parent != null) {
                if (visible || style == 0) {
                    if (visible && style == 0) {
                        removeViewFromParent();
                    } else if (this.visible && (vis & View.INVISIBLE) == 0) {
                        syncViewsZOrder();
                    }
                } else {
                    addViewToParent();
                }
            }
        }
        if (clientGroup != null) {
            clientGroup.setLayout(clientRect.left - windowRect.left, clientRect.top - windowRect.top, clientRect.right - windowRect.left, clientRect.bottom - windowRect.top);
        }
    }

    public void setZOrder(Window window) {
        int index = 0;
        parent.removeView(this);
        if (window != null) {
            index = parent.getIndexOFView(window) + 1;
        }
        parent.addView(index, this);
    }

    public void syncViewsZOrder() {
        int view = 0;
        Window window;
        View contentView;
        for (int i = parent.children.size() - 1; i >= 0; i--) {
            window = parent.getView(i);
            if (window.visible) {
                contentView = parent.clientGroup.getChildAt(view);
                if (contentView != parent.clientGroup.getContentView()) {
                    if (window == ((WindowsGroup)contentView).getWindow()) {
                        view += 1;
                    } else {
                        for (int j = i; j >= 0; j--) {
                            window = this.parent.getView(j);
                            if (window.visible) {
                                window.windowGroup.bringToFront();
                            }
                        }
                        return;
                    }
                }
            }
        }
    }

    public void setSurface(SurfaceTexture surface, boolean isClient) {
        if (isClient) {
            if (surface == null) {
                clientSurface = null;
            } else {
                if (surface != clientSurfTex) {
                    clientSurfTex = surface;
                    clientSurface = new Surface(surface);
                }
            }
            wineActivity.invoke("wine_surface_changed", hwnd, clientSurface, true);
        } else {
            if (surface == null) {
                windowSurface = null;
            } else {
                if (surface != windowSurfTex) {
                    windowSurfTex = surface;
                    windowSurface = new Surface(surface);
                }
            }
            wineActivity.invoke("wine_surface_changed", hwnd, windowSurface, false);
        }
    }

    public void setParent(Window parent, float scale)
    {
        this.scale = scale;
        if (windowGroup != null)
        {
            if (visible) {
                removeViewFromParent();
            }
            parent.createWindowGroups();
            windowGroup.setLayout(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);
        }
        this.parent.children.remove(this);
        this.parent = parent;
        this.parent.addView(this);
        if (visible && windowGroup != null) {
            addViewToParent();
        }
    }

    public int[] getEventPos(MotionEvent event)
    {
        int[] eventPos = new int[2];
        eventPos[0] = Math.round(event.getX() * scale + windowGroup.getLeft());
        eventPos[1] = Math.round(event.getY() * scale + windowGroup.getTop());
        return eventPos;
    }
}
