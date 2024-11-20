//
// Created by 24415 on 2024-11-04.
//

#include "ISubject.h"

//添加观察者
void ISubject::AddObs(IObserver *obs) {
    if (!obs)return;
    mux.lock();
    obss.push_back(obs);
    mux.unlock();
}

//通知所有观察者
void ISubject::Notify(XData data) {
    mux.lock();
    for (int i = 0; i < obss.size(); i++) {
        obss[i]->Update(data);
    }
    mux.unlock();
}
