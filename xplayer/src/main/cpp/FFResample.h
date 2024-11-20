//
// Created by 24415 on 2024-11-05.
//

#ifndef FFMPEG_FFRESAMPLE_H
#define FFMPEG_FFRESAMPLE_H


#include "IResample.h"

struct SwrContext;

class FFResample : public IResample {
public:
    bool Open(XParameter in, XParameter out = XParameter()) override;

    XData Resample(XData inData) override;

private:
    SwrContext *actx = 0;
};


#endif //FFMPEG_FFRESAMPLE_H
