//
// Created by suggie on 2022/4/3.
//
#include <iostream>
#include <csignal>
#include <semaphore.h>
#include "SentInterestTable.h"
#include "LoggerRecord.h"
#include "Logger.h"
#include "Helper.h"

extern sem_t g_sem;


void SentInterestTable::addRecord(const SentInterestRecord &record) {
    pthread_mutex_lock(&tableMutex);
    this->table.push_back(record);
    pthread_mutex_unlock(&tableMutex);
}


void SentInterestTable::matchAndDelete(const NamePrefix &np, timeval backTime, int signature) {
    pthread_mutex_lock(&tableMutex);

    //找到匹配的表项的位置
    std::vector<int> locs;
    int size = table.size();
    for (int i = 0; i < size; i++) {
        if (table[i].getNamePrefix()==np) {
            locs.push_back(i);
        }
    }
    if(locs.size()>0){
        Logger::getInstance().increaseReceivedDataPktNum();
    }

    //删除掉匹配的表项
    for (int i = locs.size() - 1; i >= 0; i--) {
        int loc = locs[i];
        //生成发包记录加入logger
        SentInterestRecord srecord = table[loc];

        //生成发包记录加入logger
        LoggerRecord loggerRecord;
        loggerRecord.setPktid(srecord.getPktid());
        loggerRecord.setNamePrefix(np);
        loggerRecord.setSendTime(srecord.getSendTime());
        loggerRecord.setBackTime(backTime);
        loggerRecord.calRTT();
        loggerRecord.setSignature(signature);
		Logger::getInstance().addRecord(loggerRecord);

        //删除掉sit中对应的表项
        table.erase(table.begin() + loc);
    }
    pthread_mutex_unlock(&tableMutex);
}


std::vector<SentInterestRecord> SentInterestTable::getTable() {
    pthread_mutex_lock(&tableMutex);
    std::vector<SentInterestRecord> ret = this->table;
    pthread_mutex_unlock(&tableMutex);
    return ret;
}

const void SentInterestTable::show() {
    pthread_mutex_lock(&tableMutex);
    printlog("=================sit的内容如下:================");
    for (auto &i: this->table) {
        printlog(i.getNamePrefix().getRouter() + "/" + i.getNamePrefix().getResource());
    }
    pthread_mutex_unlock(&tableMutex);
}

SentInterestTable::SentInterestTable() {
    printlog("SentInterestTable::SentInterestTable");
    pthread_mutex_init(&tableMutex, nullptr);
}

SentInterestTable::~SentInterestTable() {
    printlog("SentInterestTable::~SentInterestTable");
    pthread_mutex_destroy(&tableMutex);
}
