//
// Created by 24415 on 2024-11-01.
//

#ifndef FFMPEG_IOBSERVER_H
#define FFMPEG_IOBSERVER_H

#include "../XData.h"

//观察者
class IObserver {
public:
    //观察者接收数据函数
    virtual void Update(XData data) = 0;
};


#endif //FFMPEG_IOBSERVER_H
