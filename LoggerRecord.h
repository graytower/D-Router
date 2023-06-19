//
// Created by suggie on 2022/4/4.
//

#ifndef NDN_NEW_LOGGERRECORD_H
#define NDN_NEW_LOGGERRECORD_H


#include <ctime>
#include "NamePrefix.h"

class LoggerRecord {
private:
    int pktid;
    int signature;
    NamePrefix namePrefix;
    timeval sendTime;
    timeval backTime;
    int rtt;

public:
    bool operator<(const LoggerRecord &lr)const;

    const void show() const;

    const NamePrefix &getNamePrefix() const;

    void setNamePrefix(const NamePrefix &namePrefix);

    const timeval &getSendTime() const;

    void setSendTime(const timeval &sendTime);

    const timeval &getBackTime() const;

    void setBackTime(const timeval &backTime);

    int getRtt() const;

    void calRTT();

    const void printTimeval(timeval tv) const;

    int getPktid() const;

    void setPktid(int pktid);

    int getSignature() const;

    void setSignature(int signature);

};


#endif //NDN_NEW_LOGGERRECORD_H
