package com.vantacom.aarm.wine.views;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.view.TextureView;

import com.vantacom.aarm.wine.WineActivity;

public class WineView extends TextureView implements TextureView.SurfaceTextureListener {
    private Window window;
    private boolean isClient;
    private WineActivity activity;
    private org.winehq.wine.WineActivity wineActivity;

    public WineView(WineActivity activity, org.winehq.wine.WineActivity wineActivity, Context context, Window window, boolean isClient) {
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
}
