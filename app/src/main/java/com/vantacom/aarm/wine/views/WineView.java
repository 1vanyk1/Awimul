package com.vantacom.aarm.wine.views;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.util.Log;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.TextureView;

import androidx.core.view.GestureDetectorCompat;

import com.vantacom.aarm.CustomClassManager;
import com.vantacom.aarm.wine.controls.MouseActions;
import com.vantacom.aarm.wine.xserver.XServerManager;

public class WineView extends TextureView implements TextureView.SurfaceTextureListener, GestureDetector.OnGestureListener, GestureDetector.OnDoubleTapListener {
    private Window window;
    private boolean isClient;
    private XServerManager xserver;
    private CustomClassManager wineActivity;
    private GestureDetectorCompat gDetector;

    private boolean isMoving = false;
    private boolean isMultiTouch = false;

    public WineView(XServerManager xserver, Context context, Window window, boolean isClient) {
        super(context);
        this.wineActivity = xserver.getWineActivity();
        this.xserver = xserver;
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
//        if (event.getAction() == MotionEvent.ACTION_DOWN) {
//            activity.getMainView().setCurrentWindow(window, event, this.getLeft(), this.getTop());
//        }
//        if (activity.getMainView().checkWindow(window)) {
//            activity.getMainView().setEventXY(this.getX(), this.getY());
//        }
//        if (window.getGroup(false) != null  && !this.isClient) {
//            Log.e("1", String.valueOf(window.getHWND()));
//        }
        return false;
//        xserver.f();

//        xserver.f2(event.getX(), event.getY(), window);
//        Log.e("onTouchEvent", String.valueOf(window.getHWND()));
//        Log.e("onTouchEvent", String.valueOf(event.getAction()));
//        if (!xserver.isSystemPaused()) {
//            int action = event.getActionMasked();
//            if (event.getAction() == 1 && (!isMoving || !isMultiTouch) && !this.isClient && (this.window.getParent() == null || window.getParent() == xserver.getDesktopView().getDesktopWindow())) {
//                MouseActions.setLeftButtonClick(event, wineActivity, window, 1);
//            }
//            switch (action) {
//                default:
//                    if (!isMultiTouch) {
//                        return gDetector.onTouchEvent(event);
//                    }
//                    break;
//                case MotionEvent.ACTION_MOVE:
//                    if (!isMultiTouch) {
//                        return gDetector.onTouchEvent(event);
//                    }
//                    isMoving = true;
//                    if (event.getPointerCount() == 2) {
//                        xserver.getDesktopView().resize(event);
//                    }
//                    break;
//                case MotionEvent.ACTION_POINTER_DOWN:
//                    isMultiTouch = true;
//                    if (event.getPointerCount() == 2) {
//                        xserver.getDesktopView().setStartDistance(event);
//                        event.setAction(MotionEvent.ACTION_UP);
//                        gDetector.onTouchEvent(event);
//                    }
//                    break;
//                case MotionEvent.ACTION_POINTER_UP:
//                    if (isMoving) {
//                        isMoving = false;
//                    } else {
//                        xserver.getKeyboard().toggleKeyboard();
//                    }
//                    break;
//                case MotionEvent.ACTION_UP:
//                    if (!isMultiTouch) {
//                        return gDetector.onTouchEvent(event);
//                    }
//                    isMultiTouch = false;
//                    break;
//            }
//        }
//        return true;
    }

    @Override
    public boolean onDown(MotionEvent event) {
        return true;
    }

    @Override
    public void onShowPress(MotionEvent event) {}

    @Override
    public boolean onSingleTapUp(MotionEvent event) {
        if (!isMultiTouch) {
            xserver.getKeyboard().setHWND(window.getHWND());
            return MouseActions.singleLeftButtonClick(event, wineActivity, window);
        }
        return MouseActions.setLeftButtonClick(event, wineActivity, window, 2);
    }

    @Override
    public boolean onScroll(MotionEvent event1, MotionEvent event2, float distanceX, float distanceY) {
        if (window.getCanMove()) {
            if (!isMoving) {
                isMoving = true;
                MouseActions.setLeftButtonClick(event1, wineActivity, window, 0);
            } else {
                MouseActions.setLeftButtonClick(event2, wineActivity, window, 2);
            }
        }
        return true;
    }

    @Override
    public void onLongPress(MotionEvent event) {
        MouseActions.singleRightButtonClick(event, wineActivity, window);
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
