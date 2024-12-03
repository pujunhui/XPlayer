//
// Created by 24415 on 2024-11-01.
//

#include "IDecode.h"
#include "util/XLog.h"
//
////由主体notify的数据
//void IDecode::Update(XData pkt) {
//    if (pkt.isAudio != isAudio) {
//        return;
//    }
//    while (!isExit) {
//        packsMutex.lock();
//
//        //阻塞
//        if (packs.size() < maxList) {
//            //生产者
//            packs.push_back(pkt);
//            packsMutex.unlock();
//            break;
//        }
//        packsMutex.unlock();
//        XSleep(1);
//    }
//}
//
//void IDecode::Main() {
//    while (!isExit) {
//        packsMutex.lock();
//        if (packs.empty()) {
//            packsMutex.unlock();
//            XSleep(1);
//            continue;
//        }
//        //取出packet 消费者
//        XData pack = packs.front();
//        packs.pop_front();
//
//        //发送数据到解码线程，一个数据包，可能解码多个结果
//        if (this->SendPacket(pack)) {
//            while (!isExit) {
//                //获取解码数据
//                XData frame = RecvFrame();
//                if (!frame.data) break;
//                //XLOGE("RecvFrame %d",frame.size);
//
//                //发送数据给观察者
//                this->Notify(frame);
//            }
//        }else{
//
//        }
//        pack.Drop();
//        packsMutex.unlock();
//    }
//}


//由主体notify的数据
void IDecode::Update(XData pkt) {
    if (pkt.isAudio != isAudio) {
        return;
    }
    while (!isExit) {
        packsMutex.lock();
        bool success = this->SendPacket(pkt);
        packsMutex.unlock();
        if (success) {
            break;
        }
        XSleep(1);
    }
}

void IDecode::Main() {
    while (!isExit) {
        packsMutex.lock();
        XData frame = RecvFrame();
        packsMutex.unlock();
        if (frame.data) {
            //发送数据给观察者
            this->Notify(frame);
        } else {
            XSleep(1);
        }
    }
}