//
// Created by suggie on 2022/1/23.
//

#ifndef NDN_NEW_INTEREST_H
#define NDN_NEW_INTEREST_H

#include<iostream>
#include "NamePrefix.h"
#include "Pkt.h"

class Interest:public Pkt{
private:
    bool busy;
public:
    Interest(int pktid,bool busyFlag,NamePrefix np,int sign);

    Interest();

    bool isBusy() const;

    void setBusy(bool busy);

    void show() const;
};


#endif //NDN_NEW_INTEREST_H
