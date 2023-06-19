//
// Created by csy on 2023/3/12.
//

#ifndef NDN_NEW_TIMELOGMAP_H
#define NDN_NEW_TIMELOGMAP_H

#include <map>
#include <vector>
#include "TimePair.h"

class TimeLogMap {
private:
    int serviceID;
    pthread_mutex_t mutex;
    std::map<int,TimePair>  table;
    int csMatchNum;
    int bypassNum;
    int csHitNum;

public:
    TimeLogMap();

    ~TimeLogMap();

    void increaseCSMatchNum();

    void increaseBypassNum();

    void increaseCSHitNum();

    int getServiceId() const;

    void setServiceId(int serviceId);

    int getCsMatchNum() const;

    void setCsMatchNum(int csMatchNum);

    int getBypassNum() const;

    void setBypassNum(int bypassNum);

    int getCsHitNum() const;

    void setCsHitNum(int csHitNum);

    void insertByInTime(int pktid, timeval intv);

    void insert(int pktid,timeval intv, timeval outtv);

    void completeRecordById(int pktid, timeval outtv);

    void  output(const std::string& outFileName);

//    std::vector< std::pair<int,TimePair> > sortByIntime();

//    static bool cmp(std::pair<int,TimePair> a,std::pair<int,TimePair> b);

//    void outputByInTime(const std::string &outFileName);
};


#endif //NDN_NEW_TIMELOGMAP_H
