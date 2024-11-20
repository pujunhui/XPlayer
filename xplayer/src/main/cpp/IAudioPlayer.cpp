//
// Created by Administrator on 2024-11-05.
//

#include "IAudioPlayer.h"
#include "util/XLog.h"

XData IAudioPlayer::GetData() {
    while (!isExit) {
        framesMutex.lock();
        if (!frames.empty()) {
            XData d = frames.front();
            frames.pop_front();
            framesMutex.unlock();
            return d;
        }
        framesMutex.unlock();
        XSleep(1);
    }
    return XData();
}

void IAudioPlayer::Update(XData data) {
    //压入缓冲队列
    if (data.size <= 0 || !data.data) {
        return;
    }
    while (!isExit) {
        framesMutex.lock();
        if (frames.size() <= maxFrame) {
            frames.push_back(data);
            framesMutex.unlock();
            break;
        }
        framesMutex.unlock();
        XSleep(1);
    }
}

void IAudioPlayer::Main() {

}

