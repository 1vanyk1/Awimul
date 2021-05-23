package com.vantacom.aarm.wine.xserver.views;

import android.graphics.PointF;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.util.Log;
import android.view.Surface;
import android.view.View;

import com.vantacom.aarm.wine.xserver.XServerManager;

import java.util.ArrayList;

public class Window {
    private ArrayList<Window> children;
    private ArrayList<Window> ownedWindows;
    private int hwnd;
    private boolean visible;
    private float scale;
    private Window parent, ownerWindow;
    private Rect windowRect, clientRect;
    private XServerManager xserver;
    private Surface windowSurface, clientSurface;
    private WindowsGroup windowGroup, clientGroup;
    private SurfaceTexture windowSurfTex, clientSurfTex;

    private int vis = 0;
    private int next_hwnd = -3;
    private int style = 0;
    private int owner = 0;

    private boolean canMove = true;

    public Window(XServerManager xserver, int hwnd, Window parent, float scale) {
        this.xserver = xserver;
        this.hwnd = hwnd;
        this.visible = false;
        Rect rect = new Rect(0, 0, 0, 0);
        this.clientRect = rect;
        this.windowRect = rect;
        this.parent = parent;
        this.scale = scale;
        this.children = new ArrayList<Window>();
        this.ownedWindows = new ArrayList<Window>();
        if (parent != null) {
            parent.addView(this);
        }
    }

    public void addOwnedWindow(Window window) { ownedWindows.add(window); }

    public void addOwnedWindow(int index, Window window) { ownedWindows.add(index, window); }

    public int getCountOfOwnedWindow() { return ownedWindows.size(); }

    public void removeOwnedWindow(Window window) { ownedWindows.remove(window); }

    public Window getOwnedWindow(int index) { return ownedWindows.get(index); }

    public void setCanMove(boolean canMove) { this.canMove = canMove; }

    public boolean getCanMove() { return canMove; }

    public int getHWND() { return hwnd; }

    public float getScale() { return scale; }

    public Window getParent() { return parent; }

    public void addView(Window window) { children.add(window); }

    public void addView(int index, Window window) { children.add(index, window); }

    public void removeView(Window window) { children.remove(window); }

    public int getIndexOfView(Window window) { return children.indexOf(window); }

    public int getCountOfViews() { return children.size(); }

    public Window getView(int index) { return children.get(index); }

    public Rect getWindowRect() { return windowRect; }

    public Rect getClientRect() { return clientRect; }

    public int getVis() { return vis; }

    public int getNextHWND() { return next_hwnd; }

    public int getStyle() { return style; }

    public int getOwnerHWND() { return owner; }

    public Window getOwner() { return ownerWindow; }

    public SurfaceTexture getClientSurfTex() { return clientSurfTex; }

    public SurfaceTexture getWindowSurfTex() { return windowSurfTex; }

    private void removeViewFromParent() { parent.clientGroup.removeView(windowGroup); }

    public WindowsGroup getGroup(boolean isClient) {
        if (isClient) { return clientGroup; }
        return windowGroup;
    }

    public void createClientView() {
        if (clientGroup == null) { createWindowGroups(); }
        clientGroup.createContentView(true).layout(0, 0, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
    }

    public View createWindowView() {
        if (windowGroup == null) { createWindowGroups(); }
        windowGroup.createContentView(false).layout(0, 0, Math.round((windowRect.right - windowRect.left) * scale), Math.round((windowRect.bottom - windowRect.top) * scale));
        windowGroup.setScale(scale);
        return windowGroup;
    }

    public void disconnectSurface() {
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
        if (clientSurfTex != null) {
            xserver.getWineActivity().invoke("wine_surface_changed", hwnd, null, true);
        }
        if (windowSurfTex != null) {
            xserver.getWineActivity().invoke("wine_surface_changed", hwnd, null, false);
        }
    }

    public void destroy() {
        visible = false;
        if (parent != null) {
            parent.removeView(this);
        }
        if (ownerWindow != null) {
            ownerWindow.removeOwnedWindow(this);
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
            windowGroup = new WindowsGroup(xserver, this);
            clientGroup = new WindowsGroup(xserver, this);
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
        for (int i = 0; i < parent.getCountOfViews() && pos >= 0; i++) {
            window = parent.getView(i);
            if (window == this) { break; }
            if (window.visible && window == ((WindowsGroup)parent.clientGroup.getChildAt(pos)).getWindow()) {
                pos -= 1;
            }
        }
        parent.clientGroup.addView(windowGroup, pos + 1);
    }

    public void posChanged(int vis, int next_hwnd, int owner, int style, Rect clientRect, Rect windowRect, boolean changeZOrder) {
        if (canMove) {
            canMove = false;
            boolean visible = this.visible;
            this.visible = (style & 0x10000000) != 0;
            this.vis = vis;
            if (this.visible) {
                this.next_hwnd = next_hwnd;
            }
            this.style = style;
            if (ownerWindow != null) {
                ownerWindow.removeOwnedWindow(this);
            }
            this.owner = owner;
            if (this.owner != 0) {
                this.ownerWindow = xserver.getWindow(owner);
                ownerWindow.addOwnedWindow(this);
            }
            this.windowRect = windowRect;
            this.clientRect = clientRect;

            if (this.parent != null) {
                if (this.next_hwnd < 0) {
                    setZOrder(null, changeZOrder);
                } else if ((vis & View.INVISIBLE) == 0) {
                    setZOrder(xserver.getWindow(next_hwnd), changeZOrder);
                }
            }
            if (windowGroup != null) {
                windowGroup.setLayout(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);
                if (parent != null) {
                    if (visible || ((style & 0x10000000) == 0)) {
                        if (visible && ((style & 0x10000000) == 0)) {
                            removeViewFromParent();
                            if (changeZOrder) {
                                xserver.syncViewsZOrder();
                            }
                        }
                    } else {
                        addViewToParent();
                        if (changeZOrder) {
                            xserver.syncViewsZOrder();
                        }
                    }
                }
            }
            if (clientGroup != null) {
                clientGroup.setLayout(clientRect.left - windowRect.left, clientRect.top - windowRect.top, clientRect.right - windowRect.left, clientRect.bottom - windowRect.top);
            }
            canMove = true;
        }
    }

    public void setZOrder(Window window, boolean changeZOrder) {
        int index = 0;
        parent.removeView(this);
        if (changeZOrder) {
            if (window != null) {
                index = parent.getIndexOfView(window);
                xserver.changeZOrder(hwnd, window.getHWND(), true);
            } else {
                xserver.changeZOrder(hwnd, parent.getHWND(), true);
            }
            if (ownerWindow != null) {
                int currentZOrder = xserver.getZOrderByWindow(this);
                Window window1;
                ownerWindow.removeOwnedWindow(this);
                boolean added = false;
                for (int i = 0; i < ownerWindow.getCountOfOwnedWindow(); i++) {
                    window1 = ownerWindow.getOwnedWindow(i);
                    if (window1.getHWND() != hwnd && xserver.getZOrderByWindow(window1) > currentZOrder) {
                        ownerWindow.addOwnedWindow(i, this);
                        added = true;
                        break;
                    }
                }
                if (!added) {
                    ownerWindow.addOwnedWindow(this);
                }
            }
        }
        if (index >= 0) {
            parent.addView(index, this);
        }
    }

    public void setSurface(SurfaceTexture surface, boolean isClient) {
        if (isClient) {
            if (surface == null) {
                if (clientSurface != null) {
                    clientSurface.release();
                    clientSurface = null;
                }
            } else {
                if (surface != clientSurfTex) {
                    clientSurfTex = surface;
                    clientSurface = new Surface(surface);
                }
            }
            xserver.getWineActivity().invoke("wine_surface_changed", hwnd, clientSurface, true);
        } else {
            if (surface == null) {
                if (windowSurface != null) {
                    windowSurface.release();
                    windowSurface = null;
                }
            } else {
                if (surface != windowSurfTex) {
                    windowSurfTex = surface;
                    windowSurface = new Surface(surface);
                }
            }
            xserver.getWineActivity().invoke("wine_surface_changed", hwnd, windowSurface, false);
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

    public int[] getEventPos(float x, float y)
    {
        int[] eventPos = new int[2];
        eventPos[0] = Math.round(x * scale + windowGroup.getLeft());
        eventPos[1] = Math.round(y * scale + windowGroup.getTop());
        return eventPos;
    }

    public PointF convertWinCordsToDesk(PointF p) throws ClassNotFoundException {
        return convertWinCordsToDesk(p.x, p.y);
    }

    public PointF convertWinCordsToDesk(float x, float y) throws ClassNotFoundException {
        if (getGroup(false) == null) {
            throw new ClassNotFoundException();
        }
        float left = getGroup(false).getLeft();
        float top = getGroup(false).getTop();
        return new PointF(x + left, y + top);
    }

    public PointF convertDeskCordsToWin(PointF p) throws ClassNotFoundException {
        return convertDeskCordsToWin(p.x, p.y);
    }

    public PointF convertDeskCordsToWin(float x, float y) throws ClassNotFoundException {
        if (getGroup(false) == null) {
            throw new ClassNotFoundException();
        }
        float left = getGroup(false).getLeft();
        float top = getGroup(false).getTop();
        return new PointF(x - left, y - top);
    }
}
