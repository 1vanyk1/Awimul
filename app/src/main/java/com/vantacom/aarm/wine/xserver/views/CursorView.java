package com.vantacom.aarm.wine.xserver.views;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.widget.ImageView;

import com.vantacom.aarm.R;

public class CursorView extends androidx.appcompat.widget.AppCompatImageView {
    Context context;

    public CursorView(Context context, int x, int y) {
        super(context);
        this.context = context;
        Bitmap image = BitmapFactory.decodeResource(context.getResources(), R.drawable.arror);
        if (image != null) {
            setImageBitmap(image);
            layout(0, 0, image.getWidth(), image.getHeight());
            setX(x);
            setY(y);
        }
        setVisibility(INVISIBLE);
    }

    public void updatePosition(int x, int y) {
        setX(x);
        setY(y);
    }
}
