package com.vantacom.aarm.wine.views;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.view.MotionEvent;
import android.view.TextureView;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.WineActivity;

public class WineView extends TextureView implements TextureView.SurfaceTextureListener {
    private Window window;
    private boolean isClient;
    private WineActivity activity;
    private CustomClassManager wineActivity;

    public WineView(WineActivity activity, CustomClassManager wineActivity, Context context, Window window, boolean isClient) {
        super(context);
        this.wineActivity = wineActivity;
        this.activity = activity;
        this.window = window;
        this.isClient = isClient;
        setSurfaceTextureListener(this);
        setVisibility(VISIBLE);
    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        window.setSurface(surface, isClient);
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        window.setSurface(surface, isClient);
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        window.setSurface(null, isClient);
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {}

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (!this.isClient && (this.window.getParent() == null || window.getParent() == activity.getMainView().getDesktopWindow())) {
            int[] eventPos = window.getEventPos(event);
            return (boolean)wineActivity.invoke("wine_motion_event", window.getHWND(), event.getAction(), eventPos[0], eventPos[1], event.getButtonState(), 0);
        }
        return false;
    }
}
