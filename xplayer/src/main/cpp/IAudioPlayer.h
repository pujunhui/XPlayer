//
// Created by Administrator on 2024-11-05.
//

#ifndef FFMPEG_IAUDIOPLAYER_H
#define FFMPEG_IAUDIOPLAYER_H


#include "util/IObserver.h"
#include "XParameter.h"
#include "util/XThread.h"
#include <list>
#include <mutex>

class IAudioPlayer : public IObserver, public XThread {
public:
    virtual bool StartPlay(XParameter out) = 0;

    //获取缓冲数据，如果没有则阻塞
    virtual XData GetData();

    //最大缓冲
    int maxFrame = 100;
protected:
    //阻塞调用
    void Update(XData data) override;

    void Main() override;

private:
    std::list<XData> frames;
    std::mutex framesMutex;
};


#endif //FFMPEG_IAUDIOPLAYER_H
