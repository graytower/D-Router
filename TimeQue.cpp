//
// Created by csy on 2023/3/26.
//

#include <fstream>
#include "TimeQue.h"
#include "Helper.h"

void TimeQue::output(const std::string &outFileName) {
    std::fstream outFile;
    outFile.open(outFileName,std::ios::out);
    if(!outFile){
        Error("无法打开文件" + outFileName);
        exit(5);
    }
    for(auto it:timeQue){
        double milsec =
                (it.getTimePair().getOutTime().tv_sec - it.getTimePair().getInTime().tv_sec) * 1000 + (it.getTimePair().getOutTime().tv_usec -it.getTimePair().getInTime().tv_usec) / 1000;
        outFile<<it.getPktid()<<"     "<<TimevalToString(it.getTimePair().getInTime())<<"     "<<TimevalToString(it.getTimePair().getOutTime())<<"     "<<milsec <<endl;
    }
    outFile.close();
}

void TimeQue::add(TimeItem timeItem) {
    this->timeQue.push_back(timeItem);
}
