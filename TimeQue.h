//
// Created by csy on 2023/3/26.
//

#ifndef NDN_NEW_TIMEQUE_H
#define NDN_NEW_TIMEQUE_H


#include <list>
#include <string>
#include "TimeItem.h"

class TimeQue {
private:
    std::list<TimeItem> timeQue;

public:
    void add(TimeItem timeItem);

    void output(const std::string& outFileName);
};


#endif //NDN_NEW_TIMEQUE_H
