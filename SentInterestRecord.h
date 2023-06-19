//
// Created by suggie on 2022/4/3.
//

#ifndef NDN_NEW_SENTINTERESTRECORD_H
#define NDN_NEW_SENTINTERESTRECORD_H


#include <ctime>
#include "NamePrefix.h"

class SentInterestRecord {
private:
    int pktid;
    NamePrefix namePrefix;
    timeval sendTime;
    timeval backTime;

public:
    double calDuration() const;

    const NamePrefix &getNamePrefix() const;

    void setNamePrefix(const NamePrefix &namePrefix);

    const timeval &getSendTime() const;

    void setSendTime(const timeval &sendTime);

    const timeval &getBackTime() const;

    void setBackTime(const timeval &backTime);

    int getPktid() const;

    void setPktid(int pktid);
};


#endif //NDN_NEW_SENTINTERESTRECORD_H
