//
// Created by csy on 2023/3/17.
//

#include <string>
#include <fstream>
#include "RawJobsTime.h"
#include "Helper.h"



//void RawJobsTime::add(TimeItem timeItem) {
//    this->timeQue.push_back(timeItem);
//}

void RawJobsTime::output(const std::string& outFileName) {
    pthread_mutex_lock(&mutex);

    std::fstream outFile;
    outFile.open(outFileName,std::ios::out);
    if(!outFile){
        Error("无法打开文件" + outFileName);
        exit(5);
    }
    for(auto it:timeQue){
        double milsec =
                (it.getTimePair().getOutTime().tv_sec - it.getTimePair().getInTime().tv_sec) * 1000 + (it.getTimePair().getOutTime().tv_usec -it.getTimePair().getInTime().tv_usec) / 1000;
        outFile<<it.getPktid()<<"     "<<TimevalToString(it.getTimePair().getInTime())<<"     "<<TimevalToString(it.getTimePair().getOutTime())<<"     "<<milsec <<"     "<<it.getQueLength()<<endl;
    }
    outFile.close();

    pthread_mutex_unlock(&mutex);
}

void RawJobsTime::insertByInTime(timeval intv, int pktid, int quelength) {
    pthread_mutex_lock(&mutex);

    TimePair timePair{};
    timePair.setInTime(intv);

    TimeItem item{};
    item.setPktid(pktid);
    item.setTimePair(timePair);
    item.setQueLength(quelength);
    this->timeQue.push_back(item);

    pthread_mutex_unlock(&mutex);
}

void RawJobsTime::completeByPktid(int pktid, timeval outtv) {
    pthread_mutex_lock(&mutex);
    for(auto &it:this->timeQue){
        if(it.getPktid()==pktid){
            it.setOutTime(outtv);
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
}

RawJobsTime::RawJobsTime() {
    pthread_mutex_init(&mutex, nullptr);
}

RawJobsTime::~RawJobsTime() {
    pthread_mutex_destroy(&mutex);
}
