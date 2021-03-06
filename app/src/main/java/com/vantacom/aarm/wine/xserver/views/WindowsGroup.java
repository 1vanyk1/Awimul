package com.vantacom.aarm.wine.xserver.views;

import android.content.Context;
import android.view.ViewGroup;

import com.vantacom.aarm.wine.xserver.XServerManager;

public class WindowsGroup extends ViewGroup {
    private WineView contentView;
    private Window window;
    private Context context;

    public WindowsGroup(XServerManager xserver, Window window) {
        super(xserver.getContext());
        this.context = xserver.getContext();
        this.window = window;
        setVisibility(VISIBLE);
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        if (contentView != null) {
            contentView.layout(0, 0, right - left, bottom - top);
        }
    }

    public WineView createContentView(boolean isClient) {
        if (contentView != null) {
            return contentView;
        }
        contentView = new WineView(context, window, isClient);
        addView(contentView);
        return contentView;
    }

    public void destroyContentView() {
        if (contentView != null) {
            removeView(contentView);
            contentView = null;
        }
    }

    public WineView getContentView() { return contentView; }

    public Window getWindow() { return window; }

    public void setScale(float paramFloat)
    {
        if (contentView != null) {
            contentView.setPivotX(0.0F);
            contentView.setPivotY(0.0F);
            contentView.setScaleX(paramFloat);
            contentView.setScaleY(paramFloat);
        }
    }

    public void setLayout(int l, int t, int r, int b)
    {
        int left = (int)(l * window.getScale());
        int top = (int)(t * window.getScale());
        int right = (int)(r * window.getScale());
        if (right <= left + 1) {
            right = left + 2;
        }
        int bottom = (int)(b * window.getScale());
        if (bottom <= top + 1) {
            bottom = top + 2;
        }
        layout(left, top, right, bottom);
    }
}
