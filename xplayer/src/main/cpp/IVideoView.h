//
// Created by 24415 on 2024-11-04.
//

#ifndef FFMPEG_IVIDEOVIEW_H
#define FFMPEG_IVIDEOVIEW_H

#include "util/IObserver.h"
#include "XData.h"

class IVideoView : public IObserver {
public:
    virtual void Render(XData data) = 0;

    void Update(XData data) final;
};


#endif //FFMPEG_IVIDEOVIEW_H
