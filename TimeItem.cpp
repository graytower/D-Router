//
// Created by csy on 2023/3/17.
//

#include "TimeItem.h"

int TimeItem::getPktid() const {
    return pktid;
}

void TimeItem::setPktid(int pktid) {
    TimeItem::pktid = pktid;
}

const TimePair &TimeItem::getTimePair() const {
    return timePair;
}

void TimeItem::setTimePair(const TimePair &timePair) {
    TimeItem::timePair = timePair;
}

void TimeItem::setOutTime(timeval outtv) {
    timePair.setOutTime(outtv);
}

int TimeItem::getQueLength() const {
    return queLength;
}

void TimeItem::setQueLength(int queLength) {
    TimeItem::queLength = queLength;
}

void TimeItem::setInTime(timeval intv) {
    timePair.setInTime(intv);
}

