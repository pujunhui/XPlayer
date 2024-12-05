package com.pujh.xplayer;

import android.content.Context;
import android.view.Surface;

import androidx.annotation.NonNull;

import org.jetbrains.annotations.Nullable;

import java.lang.ref.WeakReference;

public class XPlayer {
    private final Context mContext;
    private long mNativeContext;

    static {
        System.loadLibrary("xplayer");
        native_init();
    }

    public XPlayer(Context context) {
        this.mContext = context;
        native_setup(new WeakReference<>(this), context.getAssets());
    }

    public void setDataSource(@NonNull String path) {
        native_setDataSource(path);
    }

    public void setSurface(@Nullable Surface surface) {
        native_setSurface(surface);
    }

    public void setDisplaySize(int width, int height) {
        native_setDisplaySize(width, height);
    }

    public void setScaleType(int scaleType) {
        native_setScaleType(scaleType);
    }

    public void start() {
        native_start();
    }

    public void stop() {
        native_stop();
    }

    public void pause() {
        native_pause();
    }

    public void resume() {
        native_resume();
    }

    public void prepare() {
        native_prepare();
    }

    public void reset() {
        native_reset();
    }

    public void release() {
        native_release();
    }

    public boolean isPlaying() {
        return native_isPlaying();
    }

    public boolean isPaused() {
        return native_isPaused();
    }

    @Override
    protected void finalize() throws Throwable {
        native_finalize();
    }

    public interface OnCompletionListener {
        void onCompletion(XPlayer player);
    }

    private OnCompletionListener mOnCompletionListener = null;

    public void setOnCompletionListener(OnCompletionListener listener) {
        mOnCompletionListener = listener;
    }

    private static void postEventFromNative(Object xplayer_ref, int what, int arg1, int arg2) {
        final XPlayer xplayer = (XPlayer) ((WeakReference) xplayer_ref).get();
        if (xplayer == null) {
            return;
        }
        OnCompletionListener listener = xplayer.mOnCompletionListener;
        if (listener != null) {
            listener.onCompletion(xplayer);
        }
    }

    private static native void native_init();

    private native void native_setup(Object player, Object assetManager);

    private native void native_release();

    private native void native_finalize();

    private native void native_setDataSource(String path);

    private native void native_setSurface(Surface surface);

    private native void native_setDisplaySize(int width, int height);

    private native void native_setScaleType(int scaleType);

    private native void native_start();

    private native void native_stop();

    private native void native_pause();

    private native void native_resume();

    private native void native_prepare();

    private native void native_reset();

    private native boolean native_isPlaying();

    private native boolean native_isPaused();
}
