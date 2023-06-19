//
// Created by csy on 2023/3/17.
//

#ifndef NDN_NEW_RAWJOBSTIME_H
#define NDN_NEW_RAWJOBSTIME_H


#include <list>
#include "TimeItem.h"

class RawJobsTime {
private:
    pthread_mutex_t  mutex;
    std::list<TimeItem> timeQue;
public:
    RawJobsTime();
    ~RawJobsTime();

//    void add(TimeItem timeItem);

    void output(const std::string& outFileName);

    void insertByInTime(timeval intv, int pktid, int quelength);

    void completeByPktid(int pktid,timeval outtv);
};


#endif //NDN_NEW_RAWJOBSTIME_H
