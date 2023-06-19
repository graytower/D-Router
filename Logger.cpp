//
// Created by suggie on 2022/4/3.
//
#include<pthread.h>
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <semaphore.h>
#include <unistd.h>
#include "Logger.h"
#include "Helper.h"
#include "Topology.h"
#include "typedef.h"

extern sem_t g_sem;
extern int keepRunning;
extern timeval startTime;

void Logger::addRecord(const LoggerRecord &record) {
    pthread_mutex_lock(&mutexLogger);
    this->pktTable.push_back(record);
    if (pktTable.size() == totalNum) {
        printlog("收到了最后一个包！");
//        show();
//        output();
        sem_post(&g_sem);
    }
    pthread_mutex_unlock(&mutexLogger);
}

double Logger::calAvgRTT() const {
    int num = pktTable.size();
    double totalRTT = 0;
    for (auto &i: pktTable) {
        totalRTT = totalRTT + i.getRtt();
    }
    double avgRTT = totalRTT / num;
    return avgRTT;
}

double Logger::calThroughput() const {
    timeval minSendTime{};
    timeval maxBackTime{};
    gettimeofday(&minSendTime, nullptr);

    maxBackTime.tv_sec = 0;
    maxBackTime.tv_usec = 0;

    for (auto &i: pktTable) {
        if (compareTimeval(i.getSendTime(), minSendTime) < 0) {
            minSendTime = i.getSendTime();
        }
        if (compareTimeval(i.getBackTime(), maxBackTime) > 0) {
            maxBackTime = i.getBackTime();
        }
    }

    double milsec =
            (maxBackTime.tv_sec - minSendTime.tv_sec) * 1000 + (maxBackTime.tv_usec - minSendTime.tv_usec) / 1000;
    int num = pktTable.size() + receivedDataPktNum;
    double throughput = num / milsec;
    return throughput;
}


double Logger::output() {
    double avgRTT = calAvgRTT();
    double throughPut = calThroughput();
    double serverHitRate = serverHitNum / (double) totalNum;
    double routerHitRate = 1 - serverHitRate;

    std::fstream f;
    f.open("Data/result/statistics.txt", std::ios::out | std::ios::app);
    f << ENABLE_NBCACHE << "     " << BUSY_THRESHOLD << "     " << OLD_ROUTER << "     " << OLD_ROUTER_SLEEP_TIME
      << "     " << ROUTER_SLEEP_TIME << "     "
      << SERVER_SLEEP_TIME << "     " << FLOW_RATE << "     " << this->serverHitNum << "     " << avgRTT << "     "
      << throughPut << "     " << serverHitRate << "     " << routerHitRate << std::endl;
    f.close();

    for (int i = 0; i < routerTimeLogs.size(); i++) {
        routerTimeLogs[i].output("Data/result/timeLog/timeLog_r" + to_string(i));
        rawJobsTimeLogs[i].output("Data/result/rawJobs/rawJobs_r" + to_string(i));
        parseTimeLogs[i].output("Data/result/parse/parse_r" + to_string(i));
    }
    serverTimeLog.output("Data/result/timeLog/timeLog_s0");
    outputPktTable("Data/result/logger.txt");
    outputPopularPktTable(
            "Data/result/popular_logger_thre/" + to_string(BUSY_THRESHOLD) + "enable" +
            to_string(ENABLE_NBCACHE) + ".txt");
    outputCSMatchBypassHitNum(
            "Data/result/csMatch_thre/csMatch_thre" + to_string(BUSY_THRESHOLD) + "enable" + to_string(ENABLE_NBCACHE)
            + "flowrate" + to_string(FLOW_RATE) + ".txt");
    return 0;
}

void Logger::show() {
    printlog("$$$$$$$$$$$$$$$$$$$$logger:$$$$$$$$$$$$$$$$$$$$");
//    sort(pktTable.begin(),pktTable.end());
    serverHitNum = 0;
    for (auto &i: this->pktTable) {
        if (i.getSignature() == Topology::getInstance().getRouterNum()) {
            serverHitNum++;
        }
//        i.show();
    }
    printlog("ENABLE_NBCACHE:" + to_string(ENABLE_NBCACHE));
    printlog("BUSY_THRESHOLD:" + to_string(BUSY_THRESHOLD));
    printlog("ROUTER_SLEEP_TIME:" + to_string(ROUTER_SLEEP_TIME));
    printlog("SERVER_SLEEP_TIME:" + to_string(SERVER_SLEEP_TIME));
    printlog("FLOW_RATE:" + to_string(FLOW_RATE));
    printlog("Logger::getInstance().size:" + to_string(this->pktTable.size()));
    printlog("serverHitNum:" + to_string(this->serverHitNum));
    printlog("receivedDataPktNum:" + to_string(this->receivedDataPktNum));
    printlog("avgRTT:" + to_string(this->calAvgRTT()));
    printlog("throughput:" + to_string(this->calThroughput()));
    double serverHitRate = serverHitNum / (double) totalNum;
    double routerHitRate = 1 - serverHitRate;
    printlog("serverHitRate:" + to_string(serverHitRate));
    printlog("routerHitRate:" + to_string(routerHitRate));
}


Logger::Logger() {
    totalNum = 0;
    serverHitNum = 0;
    pthread_mutex_init(&mutexLogger, nullptr);

    int routerNum = Topology::getInstance().getRouterNum();
    routerTimeLogs.resize(routerNum);
    for (int i = 0; i < routerNum; i++) {
        routerTimeLogs[i].setServiceId(i);
    }
    rawJobsTimeLogs.resize(routerNum);
    parseTimeLogs.resize(routerNum);

    serverTimeLog.setServiceId(routerNum);
}

Logger::~Logger() {
    pthread_mutex_destroy(&mutexLogger);
}


void Logger::addTotalNum(int addend) {
    pthread_mutex_lock(&mutexLogger);
    this->totalNum = totalNum + addend;
    pthread_mutex_unlock(&mutexLogger);
}


vector<TimeLogMap> &Logger::getRouterTimeLogs() {
    return routerTimeLogs;
}


TimeLogMap &Logger::getServerTimeLog() {
    return serverTimeLog;
}

vector<RawJobsTime> &Logger::getRawJobsTimeLogs() {
    return rawJobsTimeLogs;
}

Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

vector<TimeQue> &Logger::getParseTimeLogs() {
    return parseTimeLogs;
}

void Logger::increaseReceivedDataPktNum() {
    pthread_mutex_lock(&mutexLogger);
    Logger::receivedDataPktNum++;
    pthread_mutex_unlock(&mutexLogger);
}

const void Logger::outputPktTable(std::string fileName) {
    sort(pktTable.begin(), pktTable.end());
    fstream f;
    f.open(fileName, ios::out);
    if (f) {
        for (auto i: pktTable) {
            f << i.getPktid() << "     " << i.getNamePrefix().getRouter() + "/" + i.getNamePrefix().getResource()
              << "     " << i.getSignature() << "     " << TimevalToString(i.getSendTime()) + "      " +
                                                           TimevalToString(i.getBackTime()) + "      " +
                                                           to_string(i.getRtt())
              << endl;
        }
    } else {
        Error("can't open file " + fileName);
    }

    return;
}

const void Logger::outputPopularPktTable(std::string fileName) {
    fstream f;
    f.open(fileName, ios::out);
    if (f) {
        for (const auto &i: pktTable) {
            int requestRouterID = i.getNamePrefix().getRouter()[1];
            if (((requestRouterID % 10) >= 5) &&
                ((requestRouterID % 10) <= 9)) {
                f << i.getPktid() << "     " << i.getNamePrefix().getRouter() + "/" + i.getNamePrefix().getResource()
                  << "     " << i.getSignature() << "     " << TimevalToString(i.getSendTime()) + "      " +
                                                               TimevalToString(i.getBackTime()) + "      " +
                                                               to_string(i.getRtt())
                  << endl;
            }
        }
    } else {
        Error("can't open file " + fileName);
    }
    return;
}

const void Logger::outputCSMatchBypassHitNum(std::string fileName) {
    fstream f;
    f.open(fileName, ios::out);
    f << "serviceID       CSMatchNum     CSHitNum     CSBypassNum" << endl;
    if (f) {
        for (const auto &i: routerTimeLogs) {
            f << i.getServiceId() << "      " << i.getCsMatchNum() << "      " << i.getCsHitNum() << "      "
              << i.getBypassNum() << endl;
        }
        f << serverTimeLog.getServiceId() << "      " << serverHitNum << "      " << serverHitNum << "      "
          << 0 << endl;
    } else {
        Error("can't open file " + fileName);
    }
    return;
}





