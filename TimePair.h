//
// Created by csy on 2023/3/12.
//

#ifndef NDN_NEW_TIMEPAIR_H
#define NDN_NEW_TIMEPAIR_H


#include <ctime>

class TimePair {
private:
    timeval inTime;
    timeval outTime;
public:
    const timeval &getInTime() const;

    void setInTime(const timeval &newinTime);

    const timeval &getOutTime() const;

    void setOutTime(const timeval &newoutTime);
};


#endif //NDN_NEW_TIMEPAIR_H
