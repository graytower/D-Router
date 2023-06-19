//
// Created by suggie on 2021/11/17.
//

#ifndef NDNROUTER_CS_H
#define NDNROUTER_CS_H


#include <vector>
#include <queue>
#include <semaphore.h>
#include <algorithm>
#include "CSRecord.h"
#include "Interest.h"
#include "WaitingQueItem.h"
#include "typedef.h"



class CS {
private:
    std::queue<WaitingQueItem> waitingQueue;
    std::vector<CSRecord> records;
    int staticSize;
    int maxSize;


public:
    sem_t itemNum{};
    pthread_mutex_t queMutex{};
    pthread_mutex_t recordsMutex{};

    CS();

    ~CS();

    int getQueLen();

    void init(const std::string &configFilePath, int CSMaxSize);

    bool isBusy(const std::set<int> &exceptRouterIDs, int routerID);

    void showNamePrefixs() ;

    std::vector<NamePrefix> getNamePrefixs() ;

    bool addRecord(const CSRecord &toBeAddedRecord, int &maybeDeletedLoc);//从不被替换的那部分开始加

    void addItemToQue(WaitingQueItem item);

    WaitingQueItem getItemFromQue();

    CSRecord findRecord(NamePrefix np);

    const std::vector<CSRecord> getRecords() ;
};


#endif //NDNROUTER_CS_H
