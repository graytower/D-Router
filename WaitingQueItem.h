//
// Created by suggie on 2022/3/14.
//

#ifndef NDN_NEW_WAITINGQUEITEM_H
#define NDN_NEW_WAITINGQUEITEM_H


#include "Interest.h"
#include "Data.h"

class WaitingQueItem {
private:
    Pkt* pkt;
public:
    Pkt *getPkt() const;

private:
    int inPortID;
    timeval inTime;
    timeval outTime;
    int queLen;


public:
    WaitingQueItem();

    int getQueLen() const;

    void setQueLen(int queLen);

    void setPkt(Pkt *pkt);

    int getInPortId() const;

    void setInPortId(int inPortId);

    const timeval &getInTime() const;

    void setInTime(const timeval &inTime);

    const timeval &getOutTime() const;

    void setOutTime(const timeval &outTime);


};


#endif //NDN_NEW_WAITINGQUEITEM_H
