package org.winehq.wine;

import android.graphics.Rect;

public interface WineIStream {
    public void createDesktopWindow(int hwnd);

    public void onWineLoad();

    public void createWindow(int hwnd, boolean isClient, int parent, float scale);

    public void windowPosChanged(int hwnd, int vis, int next_hwnd, int owner, int style, Rect win_rect, Rect client_rect, Rect visible_rect);

    public void destroyWindow(int hwnd);

    public void setWindowParent(int hwnd, int hwnd_parent, float scale);
}
