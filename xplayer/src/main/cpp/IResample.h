//
// Created by 24415 on 2024-11-05.
//

#ifndef FFMPEG_IRESAMPLE_H
#define FFMPEG_IRESAMPLE_H


#include "XParameter.h"
#include "XData.h"
#include "util/IObserver.h"
#include "util/ISubject.h"

class IResample : public IObserver, public ISubject {
public:
    virtual bool Open(XParameter in, XParameter out = XParameter()) = 0;

    virtual XData Resample(XData inData) = 0;

    void Update(XData data) final;

    int outChannels = 2;
    int outFormat = 1;
};


#endif //FFMPEG_IRESAMPLE_H
