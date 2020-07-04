package com.ben.android.ngif;

import android.graphics.Bitmap;

public class GifLoader {
    private long nPtr;

    static {
        System.loadLibrary("ngif");
    }

    public void initlizate(String path) throws Exception {
        nPtr = nativeInitlizate(path);
        if (nPtr == 0) {
            throw new Exception("native initlizate error");
        }
    }

    public int getWidth() {
        return nativeGetWidth(nPtr);
    }

    public int getHeight() {
        return nativeGetHeight(nPtr);
    }

    public int render(Bitmap bitmap) {
        return nativeRender(nPtr, bitmap);
    }

    private static native long nativeInitlizate(String path);

    private static native int nativeGetWidth(long ptr);

    private static native int nativeGetHeight(long ptr);

    private static native int nativeRender(long ptr, Bitmap bitmap);
}
