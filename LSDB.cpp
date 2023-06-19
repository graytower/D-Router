//
// Created by suggie on 2022/2/23.
//

#include "LSDB.h"
#include "IndexTable.h"
#include "Helper.h"
#include "Logger.h"
#include "Topology.h"


void LSDB::addNameLSA(const NameLSA &nlsa) {
    nameLSAs.push_back(nlsa);
}

const std::vector<NameLSA> &LSDB::getNameLSAs() { ///这里记得回来看
    return nameLSAs;
}


void LSDB::setNameLSA(int loc, NameLSA newNLSA) {
    if (loc < 0 || loc > this->nameLSAs.size()) {
        printlog("setNameLSA 出错，参数 loc 越界");
        return;
    }
    this->nameLSAs[loc] = std::move(newNLSA);
}

void LSDB::updateNameLSA(const NameLSA &nlsa) {
    int size = this->nameLSAs.size();

    //若nlsa在LSDB中，则根据version号更新
    int i;
    for (i = 0; i < size; i++) {
        NameLSA temp = this->nameLSAs[i];
        if ((temp.getNetwork() == nlsa.getNetwork()) && (temp.getSite() == nlsa.getSite()) &&
            (temp.getRouter() == nlsa.getRouter()) && (temp.getNameLsa() == nlsa.getNameLsa())) {
            if (temp.getVersion() < nlsa.getVersion()) {//当前lsdb里面的NAMELSA版本比收到的old
                nameLSAs[i] = nlsa;
//                updateNamePrefixToRouterIDs(nlsa);
                break;
            }
        }

    }
    if (i == size) {
        //若nlsa不在LSDB中，则在LSDB末尾加入
        this->nameLSAs.push_back(nlsa);
//        updateNamePrefixToRouterIDs(nlsa);
    }
}


void LSDB::show() const {
//    printlog("lsdb的内容如下:");
//    for (int i = 0; i < this->nameLSAs.size(); i++) {
//        nameLSAs[i].show();
//    }
    printlog("------namePrefixsToRouterIDs如下-------------");
    for (auto &i: namePrefixToRouterIDs) {
        cout << i.first.getRouter() << "/" << i.first.getResource() << "     ";
        for (auto &id: i.second) {
            cout << id << " ";
        }
        cout << endl;
    }

}

void LSDB::updateNamePrefixToRouterIDs(const NameLSA &nlsa) {
//    pthread_mutex_lock(&mutex);
    string router = nlsa.getRouter();
    if (router[0] == 'r') {//如果是SERVER的NAME LSA则不需要同步
        string routerStr(router.begin() + 1, router.end());
        int routerID = atoi(routerStr.c_str());

        for (const auto &np: nlsa.getNamePrefixs()) {
            auto it = namePrefixToRouterIDs.find(np);
            if (it != namePrefixToRouterIDs.end()) {
                //找到,当前名称前缀已经存在，加入路由器ID号
                it->second.insert(routerID);
            } else {
                //这是一个新的名称前缀，新生成一项
                std::set<int> routerIDs;
                routerIDs.insert(routerID);
                namePrefixToRouterIDs.insert(make_pair(np, routerIDs));
            }
        }
    }
//    pthread_mutex_unlock(&mutex);
}

std::map<NamePrefix, std::set<int>> &LSDB::getNamePrefixToRouterIDs() {
    return namePrefixToRouterIDs;
}

void LSDB::addNamePrefixToID(NamePrefix np, int routerID) {
    if (routerID >= Topology::getInstance().getRouterNum()) {//不是路由器ID
        return;
    }
    auto it = namePrefixToRouterIDs.find(np);
    if (it != namePrefixToRouterIDs.end()) {
        //找到,当前名称前缀已经存在，加入路由器ID号
        it->second.insert(routerID);
    } else {
        //这是一个新的名称前缀，新生成一项
        std::set<int> routerIDs;
        routerIDs.insert(routerID);
        namePrefixToRouterIDs.insert(make_pair(np, routerIDs));
    }
}

void LSDB::deleteNamePrefixToID(NamePrefix np, int routerID) {
    auto it = namePrefixToRouterIDs.find(np);
    if (it != namePrefixToRouterIDs.end()) {//找到,当前名称前缀存在，删除
        it->second.erase(routerID);
    }
}

LSDB::LSDB() {
    pthread_mutex_init(&mutex, nullptr);
}

LSDB::~LSDB() {
    pthread_mutex_destroy(&mutex);
}

void LSDB::generateNamePrefixToRouterIDs() {
//    pthread_mutex_lock(&mutex);
    this->namePrefixToRouterIDs.clear();
    for (const auto &nlsa: nameLSAs) {
        updateNamePrefixToRouterIDs(nlsa);
    }
//    pthread_mutex_unlock(&mutex);
}





