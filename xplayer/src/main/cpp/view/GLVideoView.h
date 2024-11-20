//
// Created by 24415 on 2024-11-04.
//

#ifndef FFMPEG_GLVIDEOVIEW_H
#define FFMPEG_GLVIDEOVIEW_H

#include "../IVideoView.h"
#include "../XGLRender.h"

#include <android/native_window.h>

class GLVideoView : public IVideoView {
public:
    void Render(XData data) override;

    static void Init(ANativeWindow *window);

private:
    static ANativeWindow *mWindow;
    XGLRender *mRender = NULL;
};


#endif //FFMPEG_GLVIDEOVIEW_H
