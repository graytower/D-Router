//
// Created by 22963 on 2022/2/14.
//

#ifndef NDN_NEW_SYNC_INST_H
#define NDN_NEW_SYNC_INST_H

//周期性广播用于获取周围路由器的前缀注册信息

#include <iostream>
#include <string>
#include "NamePrefix.h"
#include "Pkt.h"

using namespace std;
class SyncInterest: public Pkt{
public:
    SyncInterest();
    SyncInterest(int pktid,NamePrefix namePrefix,int sign);

};




#endif //NDN_NEW_SYNC_INST_H
