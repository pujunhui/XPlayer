//
// Created by 24415 on 2024-11-05.
//

#include "IResample.h"

void IResample::Update(XData data) {
    XData d = this->Resample(data);
    if (d.size > 0) {
        this->Notify(d);
    }
}
