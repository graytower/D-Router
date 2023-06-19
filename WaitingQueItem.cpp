//
// Created by suggie on 2022/3/14.
//

#include <sys/time.h>
#include "WaitingQueItem.h"


int WaitingQueItem::getInPortId() const {
    return inPortID;
}

void WaitingQueItem::setInPortId(int inPortId) {
    WaitingQueItem::inPortID = inPortId;
}

WaitingQueItem::WaitingQueItem() {
    inTime.tv_sec=0;
    inTime.tv_usec=0;
    outTime.tv_sec=0;
    outTime.tv_usec=0;
    inPortID=-3;
    pkt= nullptr;
    queLen=0;
}

const timeval &WaitingQueItem::getInTime() const {
    return inTime;
}

void WaitingQueItem::setInTime(const timeval &inTime) {
    WaitingQueItem::inTime = inTime;
}

const timeval &WaitingQueItem::getOutTime() const {
    return outTime;
}

void WaitingQueItem::setOutTime(const timeval &outTime) {
    WaitingQueItem::outTime = outTime;
}


void WaitingQueItem::setPkt(Pkt *pkt) {
    WaitingQueItem::pkt = pkt;
}

Pkt *WaitingQueItem::getPkt() const {
    return pkt;
}

int WaitingQueItem::getQueLen() const {
    return queLen;
}

void WaitingQueItem::setQueLen(int queLen) {
    WaitingQueItem::queLen = queLen;
}






