//
// Created by suggie on 2022/3/21.
//

#ifndef NDN_NEW_CSFINDER_H
#define NDN_NEW_CSFINDER_H


#include "CSRecord.h"

class CSFinder {
private:
    CSRecord record;
public:
    CSFinder(const NamePrefix& np){
        record.setNamePrefix(np);
    }
    bool operator()(const CSRecord& value){
        return record.getNamePrefix()==value.getNamePrefix();
    }
};


#endif //NDN_NEW_CSFINDER_H
