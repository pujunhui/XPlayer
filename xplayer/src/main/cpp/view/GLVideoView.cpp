//
// Created by 24415 on 2024-11-04.
//

#include "GLVideoView.h"

#include "../util/XLog.h"
#include "../XEGL.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/pixdesc.h"
}

ANativeWindow *GLVideoView::mWindow = NULL;

void GLVideoView::Render(XData data) {
    if (mRender == NULL) {
        XEGL::Get()->Init(mWindow);
        mRender = XGLRender::getRender(data.format);
        mRender->Init();
    }
    mRender->Render(data.datas, data.width, data.height);
    XEGL::Get()->swapBuffers();
}

void GLVideoView::Init(ANativeWindow *window) {
    mWindow = window;
}
