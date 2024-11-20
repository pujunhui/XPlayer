//
// Created by 24415 on 2024-11-01.
//

#ifndef FFMPEG_IDECODE_H
#define FFMPEG_IDECODE_H

#include "XParameter.h"
#include "util/IObserver.h"
#include "util/ISubject.h"
#include "util/XThread.h"
#include <list>
#include <mutex>

//解码接口，支持硬解码
class IDecode : public ISubject, public XThread, public IObserver {
public:
    //打开解码器
    virtual bool Open(XParameter para, bool hard = false) = 0;

    //future模型
    //发送数据到线程解码
    virtual bool SendPacket(XData pkt) = 0;

    //从线程中获取解码结果  再次调用会复用上次空间，线程不安全
    virtual XData RecvFrame() = 0;

    bool isAudio = false;

    //最大的队列缓冲
    int maxList = 100;


protected:
    //由主体notify的数据 阻塞
    void Update(XData pkt) final;

    void Main() final;

    //读取缓冲
    std::list<XData> packs;
    std::mutex packsMutex;
};


#endif //FFMPEG_IDECODE_H
