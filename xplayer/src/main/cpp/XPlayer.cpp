//
// Created by 24415 on 2024-11-20.
//

#include "XPlayer.h"

#include "FFDemux.h"
#include "util/XLog.h"
#include "util/IObserver.h"
#include "IDecode.h"
#include "FFDecode.h"
#include "XEGL.h"

#include "IVideoView.h"
#include "view/GLVideoView.h"
#include "view/WindowVideoView.h"
#include "IResample.h"
#include "FFResample.h"
#include "XGLRender.h"
#include "IAudioPlayer.h"
#include "SLAudioPlayer.h"

class TestObs : public IObserver {
public:
    void Update(XData d) override {
        XLOGI("TestObs Update data size is %d", d.size);
//        XLOGI("TestObs Update data size is %s", d.isAudio?"audio":"video");
//        XLOGI("TestObs Update data size is %d %d", d.width, d.height);
    }
};

void XPlayer::init(AAssetManager *manager) {
    XGLRender::Init(manager);
}

void XPlayer::start() {
    IDemux *de = new FFDemux();
    de->Open(path);
    TestObs *tobs = new TestObs();
//    de->AddObs(tobs);

    IDecode *vdecode = new FFDecode();
    vdecode->Open(de->GetVPara(), false);
    de->AddObs(vdecode);

    IDecode *adecode = new FFDecode();
    adecode->Open(de->GetAPara());
    de->AddObs(adecode);

    IVideoView *view = new GLVideoView();
    vdecode->AddObs(view);

    IResample *resample = new FFResample();
    XParameter outPara = de->GetAPara();
    resample->Open(de->GetAPara(), outPara);
    adecode->AddObs(resample);

    IAudioPlayer *audioPlayer = new SLAudioPlayer();
    audioPlayer->StartPlay(outPara);
    resample->AddObs(audioPlayer);

    de->Start();
    vdecode->Start();
    adecode->Start();
//    XSleep(3000);
//    de->Stop();
}

void XPlayer::stop() {

}

void XPlayer::pause() {

}

void XPlayer::resume() {

}

void XPlayer::prepare() {
    if (mIsPrepared) {
        XLOGE("XPlayer is prepared");
        return;
    }
    mIsPrepared = true;
}

void XPlayer::reset() {
    mIsPrepared = false;
}

void XPlayer::setDataSource(const char *path) {
    if (mIsPrepared) {
        XLOGE("XPlayer is prepared");
        return;
    }
    this->path = strdup(path);
}

void XPlayer::setSurface(ANativeWindow *window) {
    GLVideoView::Init(window);
}

void XPlayer::setDisplaySize(int width, int height) {
    this->width = width;
    this->height = height;
}

void XPlayer::setScaleType(int scaleType) {
    this->scaleType = scaleType;
}

XPlayerListener *XPlayer::setListener(XPlayerListener *listener) {
    if (mListener == listener) {
        return NULL;
    }
    XPlayerListener *oldListener = mListener;
    mListener = listener;
    return oldListener;
}

bool XPlayer::isPlaying() const {
    return mIsPlaying;
}

bool XPlayer::isPaused() const {
    return mIsPaused;
}
