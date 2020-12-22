package com.vantacom.aarm.wine;

import android.content.Context;
import android.view.ViewGroup;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.views.Window;

public class MainView extends ViewGroup {
    private Window desktopWindow;
    private WineActivity activity;
    private CustomClassManager wineActivity;

    public MainView(Context context) {
        super(context);
    }

    public MainView(WineActivity activity, CustomClassManager wineActivity, Context context, int desktopView) {
        super(context);
        this.activity = activity;
        this.wineActivity = wineActivity;
        createDesktopWindow(desktopView);
    }

    public void createDesktopWindow(int desktopView) {
        desktopWindow = new Window(activity, wineActivity, desktopView, null, 1.0F);
        addView(desktopWindow.createWindowView());
        desktopWindow.getGroup(true).bringToFront();
    }

    public void destroy() {
        desktopWindow.destroy();
        desktopWindow = null;
        activity = null;
        wineActivity = null;
    }

    public Window getDesktopWindow() {
        return desktopWindow;
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {}

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        wineActivity.invoke("wine_desktop_changed", w, h);
    }
}
