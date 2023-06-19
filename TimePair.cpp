//
// Created by csy on 2023/3/12.
//

#include "TimePair.h"

const timeval &TimePair::getInTime() const {
    return inTime;
}

void TimePair::setInTime(const timeval &newinTime) {
    TimePair::inTime = newinTime;
}

const timeval &TimePair::getOutTime() const {
    return outTime;
}

void TimePair::setOutTime(const timeval &newoutTime) {
    TimePair::outTime = newoutTime;
}
