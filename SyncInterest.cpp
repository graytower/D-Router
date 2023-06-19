//
// Created by 22963 on 2022/2/14.
//

#include <cstring>
#include <utility>
#include "SyncInterest.h"
#include "Helper.h"

using namespace std;


SyncInterest::SyncInterest() {
    id = 999999;
    type=SYNC_INST;
//    routerOrServer;
    name.setResource("sync");
}

SyncInterest::SyncInterest(int pktid, NamePrefix namePrefix, int sign) {
    type=SYNC_INST;
    id=pktid;
    name=std::move(namePrefix);
    signature=sign;
}


