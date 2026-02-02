package com.virtualcontainer;

import android.view.Surface;

public class NativeBridge {
    public static native void init();
    public static native void surfaceCreated(Surface surface);
    public static native void surfaceChanged(int width, int height);
    public static native void surfaceDestroyed();
    public static native void toggleImGui();
    public static native void renderFrame();
}