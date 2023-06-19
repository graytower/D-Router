//
// Created by 22963 on 2022/2/15.
//

#include <cstring>
#include "SyncData.h"
#include "NameLSA.h"
#include "Helper.h"

using namespace std;


const vector <NameLSA> &SyncData::getNameLSAs() const {
    return nameLSAs;
}

void SyncData::setNameLSAs(const vector <NameLSA> &nameLSAs) {
    SyncData::nameLSAs = nameLSAs;
}


void SyncData::addNameLSA(const NameLSA& nlsa) {
    this->nameLSAs.push_back(nlsa);
}

void SyncData::show() {
    printlog("SyncData的内容如下");
    printlog("network:" + name.getNetwork());
    printlog("site:" + name.getSite());
    printlog("sync:" + name.getResource());
    printlog("LSANum:" + to_string(getNameLSAs().size()));
    for (int i = 0; i < getNameLSAs().size(); i++) {
        printlog("  NameLSA network:" + nameLSAs[i].getNetwork());
        printlog("  NameLSA site:" + nameLSAs[i].getSite());
        printlog("  NameLSA device:" + nameLSAs[i].getRouter());
        printlog("  NameLSA nameLsa:" + nameLSAs[i].getNameLsa());
        printlog("  NameLSA version:" + nameLSAs[i].getVersion());
        printlog("  NamePrefixNum:" + nameLSAs[i].getNamePrefixs().size());
        for (const auto & j : nameLSAs[i].getNamePrefixs()) {
            printlog("      NamePrefix network:" + j.getNetwork());
            printlog("      NamePrefix site:" + j.getSite());
            printlog("      NamePrefix device:" + j.getRouter());
            printlog("      NamePrefix resource:" + j.getResource());
        }
    }
}


SyncData::SyncData() {
    id=-999999;
    type=SYNC_DATA;
//    routerOrServer;
    name.setResource("sync");
}
