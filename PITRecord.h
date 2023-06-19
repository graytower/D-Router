//
// Created by suggie on 2022/2/23.
//

#ifndef NDN_NEW_PITRECORD_H
#define NDN_NEW_PITRECORD_H


#include <vector>
#include<set>
#include "NamePrefix.h"
#include "Port.h"

class PITRecord {
private:
    NamePrefix namePrefix;
    std::set<int> inPortsIDs;
    int outPortsIDs;
    bool busy;

public:
    const NamePrefix &getNamePrefix() const;

    void setNamePrefix(const NamePrefix &namePrefix);

    const std::set<int> &getInPortsIDs() const;

    bool getBusy() const;

    void setBusy(bool busy);

    void addInPorts(int p);
};


#endif //NDN_NEW_PITRECORD_H
