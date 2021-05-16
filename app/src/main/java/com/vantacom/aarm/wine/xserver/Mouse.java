package com.vantacom.aarm.wine.xserver;

import android.graphics.PointF;
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
        if (x < 0) {
            this.x = 0;
        } else if (x > xserver.getDesktopWidth()) {
            this.x = xserver.getDesktopWidth();
        } else {
            this.x = x;
        }
        if (y < 0) {
            this.y = 0;
        } else if (y > xserver.getDesktopHeight()) {
            this.y = xserver.getDesktopHeight();
        } else {
            this.y = y;
        }
        view.updatePosition(this.x, this.y);
    }

    public void setPosition(float x, float y) {
        setPosition((int) x, (int) y);
    }

    public void setPosition(PointF point) {
        setPosition(point.x, point.y);
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
