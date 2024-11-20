//
// Created by 24415 on 2024-11-01.
//

#ifndef FFMPEG_IDEMUX_H
#define FFMPEG_IDEMUX_H

#include "XData.h"
#include "XParameter.h"
#include "util/XThread.h"
#include "util/ISubject.h"

//解封装接口
class IDemux : public XThread, public ISubject {
public:
    //打开文件，或者流媒体 rtmp http rtsp
    virtual bool Open(const char *url) = 0;

    //获取视频参数
    virtual XParameter GetVPara() = 0;

    //获取音频参数
    virtual XParameter GetAPara() = 0;

    //读取一帧数据，数据由调用者清理
    virtual XData Read() = 0;

    //总时长(毫秒)
    int totalMs = 0;

protected:
    void Main() final;
};


#endif //FFMPEG_IDEMUX_H
