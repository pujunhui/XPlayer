//
// Created by 24415 on 2024-11-04.
//

#ifndef FFMPEG_XEGL_H
#define FFMPEG_XEGL_H


class XEGL {
public:
    virtual bool Init(void *win) = 0;

    virtual void makeCurrent() = 0;

    virtual void swapBuffers() = 0;

    static XEGL *Get();

protected:
    XEGL() {}
};


#endif //FFMPEG_XEGL_H
