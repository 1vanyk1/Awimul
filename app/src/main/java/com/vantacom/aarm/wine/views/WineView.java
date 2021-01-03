package com.vantacom.aarm.wine.views;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.TextureView;

import androidx.core.view.GestureDetectorCompat;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.WineActivity;
import com.vantacom.aarm.wine.controls.MouseActions;

public class WineView extends TextureView implements TextureView.SurfaceTextureListener, GestureDetector.OnGestureListener, GestureDetector.OnDoubleTapListener {
    private Window window;
    private boolean isClient;
    private WineActivity activity;
    private CustomClassManager wineActivity;
    private GestureDetectorCompat gDetector;

    boolean isMoving = false;

    public WineView(WineActivity activity, CustomClassManager wineActivity, Context context, Window window, boolean isClient) {
        super(context);
        this.wineActivity = wineActivity;
        this.activity = activity;
        this.window = window;
        this.isClient = isClient;
        gDetector = new GestureDetectorCompat(context,this);
        gDetector.setOnDoubleTapListener(this);
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
        if (event.getAction() == 1 && isMoving && !this.isClient && (this.window.getParent() == null || window.getParent() == activity.getMainView().getDesktopWindow())) {
            int[] eventPos = window.getEventPos(event);
            wineActivity.invoke("wine_motion_event", window.getHWND(), 1, eventPos[0], eventPos[1], 0, 0);
        }
        if (event.getAction() == MotionEvent.ACTION_POINTER_UP) {
            activity.getKeyboard().toggleKeyboard();
            return true;
        }
        return this.gDetector.onTouchEvent(event);
    }

    @Override
    public boolean onDown(MotionEvent event) {
        return true;
    }

    @Override
    public void onShowPress(MotionEvent event) {}

    @Override
    public boolean onSingleTapUp(MotionEvent event) {
        activity.getKeyboard().setHWND(window.getHWND());
        return MouseActions.singleLeftButtonClick(event, wineActivity, window);
    }

    @Override
    public boolean onScroll(MotionEvent event1, MotionEvent event2, float distanceX, float distanceY) {
        if (window.getCanMove()) {
            if (!isMoving) {
                isMoving = true;
                MouseActions.setLeftButtonClick(event2, wineActivity, window, 0);
            } else {
                MouseActions.setLeftButtonClick(event2, wineActivity, window, 2);
            }
        }
        return true;
    }

    @Override
    public void onLongPress(MotionEvent event) {
        if (this.window.getGroup(false) != null ) {
            MouseActions.singleRightButtonClick(event, wineActivity, window);
        }
    }

    @Override
    public boolean onFling(MotionEvent event1, MotionEvent event2, float velocityX, float velocityY) {
        isMoving = false;
        window.setCanMove(true);
        return MouseActions.setLeftButtonClick(event2, wineActivity, window, 1);
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent event) {
        return true;
    }

    @Override
    public boolean onDoubleTap(MotionEvent event) {
        return false;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent event) {
        return MouseActions.singleLeftButtonClick(event, wineActivity, window);
    }
}
