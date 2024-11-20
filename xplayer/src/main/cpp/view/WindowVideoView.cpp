//
// Created by 24415 on 2024-11-13.
//

#include "WindowVideoView.h"

extern "C" {
#include "libavcodec/avcodec.h"
}

ANativeWindow *WindowVideoView::mWindow = NULL;


void WindowVideoView::Render(XData data) {

}

void WindowVideoView::Init(ANativeWindow *window) {
    mWindow = window;
}

