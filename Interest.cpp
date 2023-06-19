//
// Created by suggie on 2022/1/23.
//
#include <cstring>
#include "Interest.h"
#include "PktHead.h"
#include "Helper.h"



void Interest::show() const {
    printlog("Interest的内容如下");
    printlog("busy:" + busy);
    this->getName().show();
}

bool Interest::isBusy() const {
    return busy;
}

void Interest::setBusy(bool busy) {
    Interest::busy = busy;
}

Interest::Interest() {
    type = INST;
    busy = false;
}

Interest::Interest(int pktid, bool busyFlag, NamePrefix np, int sign) {
    type = INST;
    busy = busyFlag;
    name=np;
    signature=sign;
    id=pktid;
}

