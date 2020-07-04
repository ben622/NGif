package com.ben.android.ngif.component;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.widget.AppCompatImageView;

import com.ben.android.ngif.GifLoader;

public class RenderView extends AppCompatImageView {
    private String mPath;
    private GifLoader mGifLoader;
    private Bitmap mBitmap;
    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(@NonNull Message msg) {
            //loop render.
            int delay = mGifLoader.render(mBitmap);
            if (delay == -1) {
                Toast.makeText(getContext(), "render error", Toast.LENGTH_SHORT).show();
                return;
            }
            setImageBitmap(mBitmap);
            mHandler.sendEmptyMessageDelayed(0x01, delay);
        }
    };

    public RenderView(Context context) {
        this(context,null);
    }

    public RenderView(Context context, AttributeSet attrs) {
        this(context, attrs,0);
    }

    public RenderView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public void update(String path) {
        this.mPath = path;
        initlizateGifEnvironment();
    }

    private void initlizateGifEnvironment() {
        try {
            mGifLoader = new GifLoader();
            mGifLoader.initlizate(mPath);
            int width = mGifLoader.getWidth();
            int height = mGifLoader.getHeight();
            mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
            mHandler.sendEmptyMessageDelayed(0x01, 0);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
