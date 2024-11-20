//
// Created by Administrator on 2024-11-05.
//

#ifndef FFMPEG_SLAUDIOPLAYER_H
#define FFMPEG_SLAUDIOPLAYER_H


#include "IAudioPlayer.h"

class SLAudioPlayer : public IAudioPlayer {
public:

    bool StartPlay(XParameter out) override;

    void PlayCall(void *bufq);

    SLAudioPlayer();

    virtual ~SLAudioPlayer();

protected:
    unsigned char *buf = 0;
};


#endif //FFMPEG_SLAUDIOPLAYER_H
