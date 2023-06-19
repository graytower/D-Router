//
// Created by suggie on 2022/4/3.
//

#include "SentInterestRecord.h"

const NamePrefix &SentInterestRecord::getNamePrefix() const {
    return namePrefix;
}

void SentInterestRecord::setNamePrefix(const NamePrefix &namePrefix) {
    SentInterestRecord::namePrefix = namePrefix;
}

const timeval &SentInterestRecord::getSendTime() const {
    return sendTime;
}

void SentInterestRecord::setSendTime(const timeval &sendTime) {
    SentInterestRecord::sendTime = sendTime;
}

const timeval &SentInterestRecord::getBackTime() const {
    return backTime;
}

void SentInterestRecord::setBackTime(const timeval &backTime) {
    SentInterestRecord::backTime = backTime;
}

double SentInterestRecord::calDuration() const {
    double duration = (backTime.tv_sec - sendTime.tv_sec) * 1000 + (backTime.tv_usec - sendTime.tv_usec) / 1000;
    return duration;
}

int SentInterestRecord::getPktid() const {
    return pktid;
}

void SentInterestRecord::setPktid(int pktid) {
    SentInterestRecord::pktid = pktid;
}
