//
// Created by 22963 on 2022/2/15.
//

#ifndef NDN_NEW_SYNCDATA_H
#define NDN_NEW_SYNCDATA_H

#include<iostream>
#include<vector>
#include"NameLSA.h"
#include"NamePrefix.h"
#include "Pkt.h"


using namespace std;


class SyncData:public Pkt{
private:
    vector<NameLSA> nameLSAs;

public:
    SyncData();

    void addNameLSA(const NameLSA& nlsa);
    const vector<NameLSA> &getNameLSAs() const;

    void setNameLSAs(const vector<NameLSA> &nameLSAs);

    void show();

};

#endif //NDN_NEW_SYNCDATA_H
