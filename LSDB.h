//
// Created by suggie on 2022/2/23.
//

#ifndef NDN_NEW_LSDB_H
#define NDN_NEW_LSDB_H


#include <vector>
#include <queue>
#include <set>
#include <map>
#include "NameLSA.h"
#include "NamePrefix.h"
#include "DeviceName.h"
#include "SyncData.h"
#include "IndexTable.h"

class LSDB {
private:
    std::vector<NameLSA> nameLSAs;
    std::map<NamePrefix, std::set<int>> namePrefixToRouterIDs;//知道每一个名字被哪些路由器拥有
public:
    pthread_mutex_t mutex;

public:
    LSDB();

    ~LSDB();

    void setNameLSA(int loc, NameLSA newNLSA);

    void show() const;

    void addNameLSA(const NameLSA &nlsa);

    void updateNameLSA(const NameLSA &nlsa);

    void addNamePrefixToID(NamePrefix np,int routerID);

    void deleteNamePrefixToID(NamePrefix np,int routerID);

    void generateNamePrefixToRouterIDs();

    const std::vector<NameLSA> &getNameLSAs();

    std::map<NamePrefix, std::set<int>> &getNamePrefixToRouterIDs();

private:
    void updateNamePrefixToRouterIDs(const NameLSA &nlsa);
};


#endif //NDN_NEW_LSDB_H
