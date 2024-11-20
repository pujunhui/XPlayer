//
// Created by 24415 on 2024-11-20.
//

#ifndef XPLAYER_XPLAYER_H
#define XPLAYER_XPLAYER_H


#include <android/native_window.h>

class XPlayer {
public:
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

    void setListener(void *listener);

private:
    bool mIsPrepared = false;
    bool mIsPlaying = false;
    bool mIsPaused = false;

private:
    char *path = NULL;
    ANativeWindow *window = NULL;
    int width, height;
    int scaleType;
};


#endif //XPLAYER_XPLAYER_H
