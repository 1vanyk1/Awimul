package com.vantacom.aarm.wine.xserver;

import android.view.View;

import com.vantacom.aarm.wine.xserver.views.CursorView;

public class Mouse {
    private int x, y;
    private XServerManager xserver;
    private CursorView view;

    public Mouse(XServerManager xserver, int x, int y) {
        this.xserver = xserver;
        this.x = x;
        this.y = y;
        this.view = new CursorView(xserver.getContext(), x, y);
    }

    public float getX() {
        return x;
    }

    public float getY() {
        return y;
    }

    public void setPosition(int x, int y) {
        this.x = x;
        this.y = y;
        view.updatePosition(x, y);
    }

    public void setPosition(float x, float y) {
        setPosition((int) x, (int) y);
    }

    public CursorView getView() {
        return view;
    }

    public void setVisibility(boolean visible) {
        if (visible) {
            view.setVisibility(View.VISIBLE);
        } else {
            view.setVisibility(View.INVISIBLE);
        }
    }
}
