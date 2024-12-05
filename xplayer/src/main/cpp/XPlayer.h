//
// Created by 24415 on 2024-11-20.
//

#ifndef XPLAYER_XPLAYER_H
#define XPLAYER_XPLAYER_H


#include <android/native_window.h>
#include <android/asset_manager.h>
#include "utils/Synchronizer.h"

class XPlayerListener {
public:
    virtual void post_event(int msg, int ext1, int ext2) = 0;
};

class XPlayer {
public:
    void init(AAssetManager *manager);

    void start();

    void stop();

    void pause();

    void resume();

    void prepare();

    void reset();

    void setDataSource(const char *path);

    void setSurface(ANativeWindow *window);

    void setDisplaySize(int width, int height);

    void setScaleType(int scaleType);

    XPlayerListener *setListener(XPlayerListener *listener);

    bool isPlaying() const;

    bool isPaused() const;

private:
    bool mIsPrepared = false;
    bool mIsPlaying = false;
    bool mIsPaused = false;

private:
    char *path = NULL;
    int width, height;
    int scaleType;
    XPlayerListener *mListener = NULL;
    Synchronizer *synchronizer = NULL;
};


#endif //XPLAYER_XPLAYER_H
