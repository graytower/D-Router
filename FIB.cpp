//
// Created by suggie on 2022/2/23.
//

#include <iostream>
#include "FIB.h"
#include "Helper.h"

void FIB::clear() {
    fibMap.clear();
}



FIB::FIB() {
    printlog("FIB::FIB");
    pthread_mutex_init(&tableMutex, nullptr);
}

FIB::~FIB() {
    printlog("FIB::~FIB");
    pthread_mutex_destroy(&tableMutex);
}



//insertRecord不能上锁，会锁中锁
void FIB::insertRecord(std::pair<NamePrefix, std::pair<int, int>> fibRecord) {
    fibMap.insert(fibRecord);
}

FIB_MAP::iterator FIB::match(const NamePrefix &np) {
    pthread_mutex_lock(&tableMutex);
   auto  it=fibMap.find(np);
    pthread_mutex_unlock(&tableMutex);
   return it;
}

const FIB_MAP &FIB::getFibMap() const {
    return fibMap;
}
