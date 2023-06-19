//
// Created by suggie on 2021/11/17.
//

#include <fstream>
#include <set>
#include <sys/time.h>
#include "CS.h"
#include "ConfigFileInStream.h"
#include "CSFinder.h"
#include "Helper.h"

extern int BUSY_THRESHOLD;


bool CS::addRecord(const CSRecord &toBeAddedRecord, int &maybeDeletedLoc) {
//    pthread_mutex_lock(&recordsMutex);
    bool ret;

    NamePrefix np = toBeAddedRecord.getNamePrefix();
    if (std::find_if(records.begin(), records.end(), CSFinder(np)) == this->records.end()) //没有重复表项才添加记录
    {
        if (records.size() == maxSize) {
            maybeDeletedLoc = staticSize;
//            printlog("CS is full, maybeDeletedLoc is" + to_string(maybeDeletedLoc) + "record is:"
//                     + records[maybeDeletedLoc].getNamePrefix().getRouter() + "/" +
//                     records[maybeDeletedLoc].getNamePrefix().getResource());
            records.erase(records.begin() + maybeDeletedLoc); //满了，删除第一个不固定的表项
        }
        records.push_back(toBeAddedRecord);
        ret = true;
    } else {
        ret = false;
    }

//    pthread_mutex_unlock(&recordsMutex);
    return ret;
}

const std::vector<CSRecord> CS::getRecords() { ///这个要不要加锁呀？
//    pthread_mutex_lock(&recordsMutex);
//    const std::vector<CSRecord> ans = this->records;
//    pthread_mutex_unlock(&recordsMutex);
    return this->records;;
}


void CS::showNamePrefixs() {
    pthread_mutex_lock(&recordsMutex);
    printlog("@@@@@@@@@@@@@@@@@@@@CS的内容如下@@@@@@@@@@@@@@@@@");
    for (const auto &record: records) {
        record.getNamePrefix().show();
    }
    pthread_mutex_unlock(&recordsMutex);
}

bool CS::isBusy(const set<int> &exceptRouterIDs, int routerID) {
    if(exceptRouterIDs.find(routerID)!=exceptRouterIDs.end()){//serviceID is in exceptRouterIDs
//        printlog("$$$$$$$$"+ to_string(serviceID)+"CS  isn't busy");
        return false;
    }

    pthread_mutex_lock(&queMutex);

    long long size = waitingQueue.size();
    bool ret = false;
    if (size >= BUSY_THRESHOLD) {
//        printlog("waitingQueue.size is " + to_string(waitingQueue.size()) + ", CS is busy!!");
        ret = true;
    }

    pthread_mutex_unlock(&queMutex);

    return ret;
}


void CS::addItemToQue(WaitingQueItem item) {
    pthread_mutex_lock(&queMutex);

    timeval tv{};
    gettimeofday(&tv, nullptr);
    item.setInTime(tv);
    item.setQueLen(waitingQueue.size());
    waitingQueue.push(item);
//    printlog("!!!!!!!!!!!!!!!!!!!!向CS队列里放入一个Item,waitingQueue.size is" + to_string(waitingQueue.size()));
    pthread_mutex_unlock(&queMutex);

    sem_post(&itemNum);
}

WaitingQueItem CS::getItemFromQue() {
    sem_wait(&itemNum);        //等待waitingQueue的来包信号

    pthread_mutex_lock(&queMutex);
    WaitingQueItem item;
    if (!waitingQueue.empty()) {
//        item.setQueLen(waitingQueue.size());
        item = waitingQueue.front();
        waitingQueue.pop();

        timeval tv{};
        gettimeofday(&tv, nullptr);
        item.setOutTime(tv);
    }
    pthread_mutex_unlock(&queMutex);

    return item;
}


void CS::init(const std::string &configFilePath, int CSMaxSize) {

    //设置CS的最大容量
//    this->maxSize = CSMaxSize;

    //注册名称前缀
    std::vector<NamePrefix> nps = ConfigFileInStream::readInNamePrefixs(configFilePath);
    for (auto &np: nps) {
        CSRecord csRecord;
        csRecord.setNamePrefix(np);
        csRecord.setContent(
                "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");//随便设一些数据
        this->records.push_back(csRecord);
//        if (this->records.size() >= CSMaxSize) {
//            break;
//        }
    }

    //去重
    std::set<CSRecord> st(records.begin(), records.end());
    records.assign(st.begin(), st.end());

    //给不被替换的大小赋初值
    this->staticSize = records.size();

    this->maxSize=staticSize+50;

}

CS::CS() {
    printlog("CS::CS");
    this->maxSize = 0;
    this->staticSize = 0;
    pthread_mutex_init(&this->queMutex, nullptr); //初始化waitingQue互斥信号量
    pthread_mutex_init(&this->recordsMutex, nullptr);//初始化表的互斥信号量
    sem_init(&this->itemNum, 0, 0);    //初始化排队计数信号量
}

CS::~CS() {
    printlog("CS::~CS");
    pthread_mutex_destroy(&queMutex);
    pthread_mutex_destroy(&recordsMutex);
    sem_destroy(&itemNum);
}


std::vector<NamePrefix> CS::getNamePrefixs() {
    pthread_mutex_lock(&recordsMutex);

    std::vector<NamePrefix> namePrefixs;
    for (auto &r: records) {
        namePrefixs.push_back(r.getNamePrefix());
    }

    pthread_mutex_unlock(&recordsMutex);

    return namePrefixs;
}


CSRecord CS::findRecord(NamePrefix np) {
    pthread_mutex_lock(&recordsMutex);

    CSRecord csRecord;
    for (int i = 0; i < records.size(); i++) {
        if (records[i].getNamePrefix() == np) {
            csRecord = records[i];
            break;
        }
    }
    pthread_mutex_unlock(&recordsMutex);

    return csRecord;
}

int CS::getQueLen() {
    pthread_mutex_lock(&queMutex);
    int len=this->waitingQueue.size();
    pthread_mutex_unlock(&queMutex);
    return len;
}





//bool CS::getBusy(int busyRate) {
//    int num = generateRandom(100);
//    if (num < busyRate) {
////        if (waitingQueue.size() >= BUSY_THRESHOLD) {
//        return true;
//    } else {
//        return false;
//    }
//}