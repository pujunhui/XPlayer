//
// Created by 24415 on 2024-11-04.
//

#ifndef FFMPEG_ISUBJECT_H
#define FFMPEG_ISUBJECT_H

#include <vector>
#include <mutex>
#include "IObserver.h"
#include "../XData.h"

//订阅对象
class ISubject {
public:
    //添加观察者(线程安全)
    void AddObs(IObserver *obs);

    //通知所有观察者(线程安全)
    void Notify(XData data);

private:
    std::vector<IObserver *> obss;
    std::mutex mux;
};


#endif //FFMPEG_ISUBJECT_H
