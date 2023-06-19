//
// Created by csy on 2023/3/17.
//

#ifndef NDN_NEW_TIMEITEM_H
#define NDN_NEW_TIMEITEM_H


#include "TimePair.h"

class TimeItem {
private:
    int pktid;
    TimePair timePair;
    int queLength;//记录入队的时候rawjobsQue的长度

public:
    int getQueLength() const;

    void setQueLength(int queLength);

    int getPktid() const;

    void setPktid(int pktid);

    const TimePair &getTimePair() const;

    void setTimePair(const TimePair &timePair);

    void setOutTime(timeval outtv);

    void setInTime(timeval intv);
};


#endif //NDN_NEW_TIMEITEM_H
