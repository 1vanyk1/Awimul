package com.vantacom.aarm.wine.xserver.views;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.view.TextureView;

public class WineView extends TextureView implements TextureView.SurfaceTextureListener {
    private Window window;
    private boolean isClient;

    public WineView(Context context, Window window, boolean isClient) {
        super(context);
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
