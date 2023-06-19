//
// Created by suggie on 2022/2/23.
//

#ifndef NDN_NEW_PIT_H
#define NDN_NEW_PIT_H


#include <vector>
#include "PITRecord.h"

class PIT {
private:
    std::vector<PITRecord> table;
public:
    const std::vector<PITRecord> &getTable() const;

public:
    bool addPITRecord(NamePrefix interestName, int inPortID, bool instBusy);

    void deleteRecordByNamePrefix(NamePrefix np);

    const PITRecord findRecord(const NamePrefix &np)const;
};


#endif //NDN_NEW_PIT_H
