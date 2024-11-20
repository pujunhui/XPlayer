//
// Created by 24415 on 2024-11-01.
//

#include "XThread.h"
#include "XLog.h"

#include <thread>

using namespace std;

void XSleep(int mis) {
    chrono::milliseconds du(mis);
    this_thread::sleep_for(du);
}

//启动线程
void XThread::Start() {
    isExit = false;
    thread th(&XThread::ThreadMain, this);
    //在C++中，std::thread::detach()函数用于将一个线程从其std::thread对象中分离出来，使其成为一个后台线程或守护线程。
    //一旦线程被分离，原始的std::thread对象就不再与该线程相关联。当进程退出时，这个后台线程会被pcb回收，不会报错。
    th.detach();
}

void XThread::ThreadMain() {
    isRunning = true;
    XLOGI("线程函数进入");
    Main();
    XLOGI("线程函数退出");
    isRunning = false;
}

//通过控制isExit安全停止线程（不一定成功）
void XThread::Stop() {
    XLOGI("Stop 停止线程begin!");
    isExit = true;
    for (int i = 0; i < 200; i++) {
        if (!isRunning) {
            XLOGI("Stop 线程停止成功!");
            return;
        }
        XSleep(1);
    }
    XLOGI("Stop 停止线程超时!");
}

