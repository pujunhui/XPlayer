//
// Created by 24415 on 2024-11-01.
//

#ifndef FFMPEG_FFDEMUX_H
#define FFMPEG_FFDEMUX_H


#include "IDemux.h"

struct AVFormatContext;

class FFDemux : public IDemux {
public:
    FFDemux();

    //打开文件，或者流媒体 rtmp http rtsp
    bool Open(const char *url) override;

    //获取视频参数
    XParameter GetVPara() override;

    //获取音频参数
    XParameter GetAPara() override;

    //读取一帧数据，数据由调用者清理
    XData Read() override;

private:
    AVFormatContext *ic = NULL; //c++11,无参构造函数才会生效
    int audioStream = 1;
    int videoStream = 0;
};


#endif //FFMPEG_FFDEMUX_H
