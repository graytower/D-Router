//
// Created by suggie on 2022/2/23.
//

#include <vector>
#include<algorithm>
#include<iostream>
#include "PITRecord.h"

const NamePrefix &PITRecord::getNamePrefix() const {
    return namePrefix;
}

void PITRecord::setNamePrefix(const NamePrefix &namePrefix) {
    PITRecord::namePrefix = namePrefix;
}

const std::set<int> &PITRecord::getInPortsIDs() const {
    return inPortsIDs;
}


void PITRecord::addInPorts(int p) {
    inPortsIDs.insert(p);
}

bool PITRecord::getBusy() const {
    return busy;
}

void PITRecord::setBusy(bool busy) {
    PITRecord::busy = busy;
}


