//
// Created by suggie on 2022/4/3.
//

#ifndef NDN_NEW_SENTINTERESTTABLE_H
#define NDN_NEW_SENTINTERESTTABLE_H

#include<vector>
#include "SentInterestRecord.h"

class SentInterestTable {
private:
    std::vector<SentInterestRecord> table;

public:
    pthread_mutex_t tableMutex; //sit要加锁的原因是 host的 recv和send两个线程都用到了

    SentInterestTable();

    ~SentInterestTable();

    const void show() ;

    std::vector<SentInterestRecord> getTable() ;

    void addRecord(const SentInterestRecord &record);


    void matchAndDelete(const NamePrefix &np, timeval backTime, int signature);
};


#endif //NDN_NEW_SENTINTERESTTABLE_H
