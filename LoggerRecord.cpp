//
// Created by suggie on 2022/4/4.
//

#include <iostream>
#include <fstream>
#include "LoggerRecord.h"
#include "Helper.h"

const timeval &LoggerRecord::getSendTime() const {
    return sendTime;
}

void LoggerRecord::setSendTime(const timeval &sendTime) {
    LoggerRecord::sendTime = sendTime;
}

const timeval &LoggerRecord::getBackTime() const {
    return backTime;
}

void LoggerRecord::setBackTime(const timeval &backTime) {
    LoggerRecord::backTime = backTime;
}

int LoggerRecord::getRtt() const {
    return rtt;
}

void LoggerRecord::calRTT() {
    this->rtt = (backTime.tv_sec - sendTime.tv_sec) * 1000 + (backTime.tv_usec - sendTime.tv_usec) / 1000;

}

const NamePrefix &LoggerRecord::getNamePrefix() const {
    return namePrefix;
}

void LoggerRecord::setNamePrefix(const NamePrefix &namePrefix) {
    LoggerRecord::namePrefix = namePrefix;
}


const void LoggerRecord::show() const {
    std::cout<<pktid<<"     "<<signature<<"     "<<namePrefix.getRouter()+"/"+namePrefix.getResource()+"      ";
    printTimeval(sendTime);
    printTimeval(backTime);
    std::cout<<rtt<<std::endl;
}

const void LoggerRecord::printTimeval(timeval tv)const{
    char timeStr[40]={0};
    struct tm* ptm = localtime (&tv.tv_sec);
    strftime (timeStr, sizeof (timeStr), "%Y-%m-%d %H:%M:%S", ptm);
    long milliseconds = tv.tv_usec / 1000;
    /* 以秒为单位打印格式化后的时间日期，小数点后为毫秒。*/
    printf ("%s.%03ld\t", timeStr, milliseconds);
}

int LoggerRecord::getPktid() const {
    return pktid;
}

void LoggerRecord::setPktid(int pktid) {
    LoggerRecord::pktid = pktid;
}

bool LoggerRecord::operator<(const LoggerRecord &lr) const {
    bool ret;
    if(this->pktid<lr.pktid){
      ret= true;
    } else{
        ret= false;
    }
    return ret;
}

int LoggerRecord::getSignature() const {
    return signature;
}

void LoggerRecord::setSignature(int signature) {
    LoggerRecord::signature = signature;
}
