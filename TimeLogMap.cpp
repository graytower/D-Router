//
// Created by csy on 2023/3/12.
//

#include <fstream>
#include <algorithm>
#include "TimeLogMap.h"
#include "Helper.h"


void TimeLogMap::output(const std::string &outFileName) {
    pthread_mutex_lock(&mutex);
    std::fstream outFile;
    outFile.open(outFileName, std::ios::out);
    if (!outFile) {
        Error("无法打开文件" + outFileName);
        exit(5);
    }

    outFile << "csMatchNum:" << to_string(csMatchNum) << endl;
    outFile << "bypassNum:" << to_string(bypassNum) << endl;
    outFile << "csHitNum" << to_string(csHitNum) << endl;
    for (auto it: table) {
        double milsec =
                (it.second.getOutTime().tv_sec - it.second.getInTime().tv_sec) * 1000 +
                (it.second.getOutTime().tv_usec - it.second.getInTime().tv_usec) / 1000;
        outFile << it.first << "     " << TimevalToString(it.second.getInTime()) << "     "
                << TimevalToString(it.second.getOutTime()) << "     " << milsec << endl;
    }
    outFile.close();
    pthread_mutex_unlock(&mutex);
}

void TimeLogMap::insertByInTime(int pktid, timeval intv) {
    pthread_mutex_lock(&mutex);
    TimePair tp{};
    tp.setInTime(intv);
    table.insert(make_pair(pktid, tp));
    pthread_mutex_unlock(&mutex);
}

void TimeLogMap::completeRecordById(int pktid, timeval outtv) {
    pthread_mutex_lock(&mutex);
    table[pktid].setOutTime(outtv);
    pthread_mutex_unlock(&mutex);
}

void TimeLogMap::insert(int pktid, timeval intv, timeval outtv) {
    pthread_mutex_lock(&mutex);
    TimePair tp{};
    tp.setInTime(intv);
    tp.setOutTime(outtv);
    table.insert(make_pair(pktid, tp));
    pthread_mutex_unlock(&mutex);
}

//std::vector<std::pair<int, TimePair> > TimeLogMap::sortByIntime() {
//    vector<std::pair<int, TimePair>> vec;
//    for(auto it:this->table)
//    {
//        vec.emplace_back(it.first,it.second);
//    }
//    sort(vec.begin(),vec.end(),cmp);
//    return vec;
//}

//void TimeLogMap::outputByInTime(const std::string& outFileName){
//    std::fstream outFile;
//    outFile.open(outFileName,std::ios::out);
//    if(!outFile){
//        Error("无法打开文件" + outFileName);
//        exit(5);
//    }
//
//    vector<std::pair<int, TimePair>> vec=sortByIntime();
//    for(auto it:vec){
//        double t= calTimeDif(it.second.getOutTime(),it.second.getInTime());
//        outFile<<it.first<<"     "<<TimevalToString(it.second.getInTime())<<"     "<<TimevalToString(it.second.getOutTime())<<"     "<<t <<endl;
//    }
//}

//bool TimeLogMap::cmp(pair<int,TimePair> a, pair<int,TimePair> b){
//    return compareTimeval(a.second.getInTime(),b.second.getInTime())<0;
//}

TimeLogMap::TimeLogMap() {
    csMatchNum = 0;
    bypassNum = 0;
    csHitNum = 0;
    pthread_mutex_init(&mutex, nullptr);
}

TimeLogMap::~TimeLogMap() {
    pthread_mutex_unlock(&mutex);
}

int TimeLogMap::getCsMatchNum() const {
    return csMatchNum;
}

void TimeLogMap::setCsMatchNum(int csMatchNum) {
    TimeLogMap::csMatchNum = csMatchNum;
}

int TimeLogMap::getBypassNum() const {
    return bypassNum;
}

void TimeLogMap::setBypassNum(int bypassNum) {
    TimeLogMap::bypassNum = bypassNum;
}

int TimeLogMap::getCsHitNum() const {
    return csHitNum;
}

void TimeLogMap::setCsHitNum(int csHitNum) {
    TimeLogMap::csHitNum = csHitNum;
}

void TimeLogMap::increaseCSMatchNum() {
    csMatchNum++;
}

void TimeLogMap::increaseBypassNum() {
    bypassNum++;
}

void TimeLogMap::increaseCSHitNum() {
    csHitNum++;
}

int TimeLogMap::getServiceId() const {
    return serviceID;
}

void TimeLogMap::setServiceId(int serviceId) {
    serviceID = serviceId;
}
