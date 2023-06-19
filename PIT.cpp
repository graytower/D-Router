//
// Created by suggie on 2022/2/23.
//

#include <vector>
#include <algorithm>
#include <iostream>
#include "PIT.h"
#include "Helper.h"

//该interest需要转发置true,不需要转发置false
bool PIT::addPITRecord(NamePrefix interestName, int inPortID, bool instBusy) {
    bool forward= false;
    int num = this->table.size();
    //合并表项
    for (int i = 0; i < num; i++) {
        if (interestName==table[i].getNamePrefix()) {
            table[i].addInPorts(inPortID);
            if(!table[i].getBusy() && instBusy){
                table[i].setBusy(true);
                forward=true;
            }else{
                forward= false;
            }
            return forward;
        }
    }

    //新建表项
    PITRecord pitRecord;
    pitRecord.setNamePrefix(interestName);
    pitRecord.addInPorts(inPortID);
    pitRecord.setBusy(instBusy);
    table.push_back(pitRecord);
    forward= true;
    return forward;
}


void PIT::deleteRecordByNamePrefix(NamePrefix np) {

    for (auto it = table.begin(); it != table.end(); it++) {
        if (it->getNamePrefix()==np) {
            table.erase(it);
            break;
        }
    }
}


const PITRecord PIT::findRecord(const NamePrefix &np) const {

    PITRecord record;
    for (auto i=0;i<table.size();i++) {
        if(table[i].getNamePrefix()==np){
            record=table[i];
            break;
        }
    }

    return record;
}

const vector<PITRecord> &PIT::getTable() const {
    return table;
}


