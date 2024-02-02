package com.vantacom.aarm.wine.xserver.views;

import android.content.Context;
import android.util.Log;
import android.view.ViewGroup;

import com.vantacom.aarm.wine.xserver.XServerManager;
import com.vantacom.aarm.xserver.WM;

public class ScreenGroupView extends ViewGroup {
    private ScreenView screenView;
    private WM wm;
    private Context context;

    public ScreenGroupView(XServerManager xserver, WM wm) {
        super(xserver.getContext());
        this.context = xserver.getContext();
        this.wm = wm;
        setVisibility(VISIBLE);
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        Log.e("onLayout", "onLayout");
        if (screenView != null) {
            screenView.layout(0, 0, right - left, bottom - top);
        }
    }

    public ScreenView createContentView() {
        if (screenView != null) {
            return screenView;
        }
        screenView = new ScreenView(context, wm);
        addView(screenView);
        return screenView;
    }

    public void destroyContentView() {
        if (screenView != null) {
            removeView(screenView);
            screenView = null;
        }
    }

    public ScreenView getContentView() { return screenView; }

    public void setScale(float paramFloat)
    {
        if (screenView != null) {
            screenView.setPivotX(0.0F);
            screenView.setPivotY(0.0F);
            screenView.setScaleX(paramFloat);
            screenView.setScaleY(paramFloat);
        }
    }

    public void setLayout(int left, int top, int right, int bottom)
    {
        if (right <= left + 1) {
            right = left + 2;
        }
        if (bottom <= top + 1) {
            bottom = top + 2;
        }
        layout(left, top, right, bottom);
    }
}
