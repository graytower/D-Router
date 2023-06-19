//
// Created by suggie on 2022/4/17.
//
#include <algorithm>
#include <iostream>
#include "IndexTable.h"
#include "Helper.h"

const std::vector<NamePrefix> &IndexTable::getNamePrefixs() const {
    return namePrefixs;
}

void IndexTable::setNamePrefixs(const std::vector<NamePrefix> &namePrefixs) {
    IndexTable::namePrefixs = namePrefixs;
}

bool IndexTable::find(const NamePrefix &np) {
    bool ret= false;
    auto it=std::find(namePrefixs.begin(), namePrefixs.end(), np);
    if(it!=namePrefixs.end()){
        ret= true;
    }
    return ret;
}

int IndexTable::getVersion() const {
    return version;
}

void IndexTable::setVersion(int version) {
    IndexTable::version = version;
}

IndexTable::IndexTable() {
    this->version=0;
}

void IndexTable::show() {
    printlog("############IndexTable的内容如下###########");
    for(auto &np:namePrefixs){
        np.show();
    }
}
