//
// Created by suggie on 2022/4/3.
//

#ifndef NDN_NEW_LOGGER_H
#define NDN_NEW_LOGGER_H

#include <vector>
#include "SentInterestRecord.h"
#include "LoggerRecord.h"
#include "TimeLogMap.h"
#include "RawJobsTime.h"
#include "TimeQue.h"


class Logger {
private:
    Logger();
    ~Logger();

public:
    static Logger &getInstance();// 成员变量

private:
    std::vector<LoggerRecord> pktTable;
    int serverHitNum;
    int receivedDataPktNum; //主机收到的Data包的数量
    int totalNum;
    pthread_mutex_t mutexLogger;//因为有多个主机访问Logger
    std::vector<TimeLogMap> routerTimeLogs;
    std::vector<RawJobsTime> rawJobsTimeLogs;
    std::vector<TimeQue> CSTimeLbogs;
    std::vector<TimeQue> parseTimeLogs;
    TimeLogMap serverTimeLog;

public:
    void show();

    double output();

    void addRecord(const LoggerRecord &record);

    void increaseReceivedDataPktNum();

    //计算平均RTT
    double calAvgRTT() const;

    //计算吞吐量
    double calThroughput() const;

    //输出到文件
    void addTotalNum(int addend);

    std::vector<TimeLogMap> &getRouterTimeLogs();

    TimeLogMap &getServerTimeLog();

    std::vector<RawJobsTime> &getRawJobsTimeLogs();

    std::vector<TimeQue> &getParseTimeLogs();

    const void outputPktTable(std::string fileName);

    const void outputPopularPktTable(std::string fileName);

    const void outputCSMatchBypassHitNum(std::string fileName);

};


#endif //NDN_NEW_LOGGER_H
