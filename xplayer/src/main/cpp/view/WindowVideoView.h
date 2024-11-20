//
// Created by 24415 on 2024-11-13.
//

#ifndef FFMPEG_WINDOWVIDEOVIEW_H
#define FFMPEG_WINDOWVIDEOVIEW_H

#include "../IVideoView.h"

#include <android/native_window.h>


class WindowVideoView : public IVideoView {
public:
    void Render(XData data) override;

    static void Init(ANativeWindow *window);

private:
    static ANativeWindow *mWindow;
};


#endif //FFMPEG_WINDOWVIDEOVIEW_H
