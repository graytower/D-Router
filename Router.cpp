//
// Created by suggie on 2022/2/4
//

#include <vector>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <sys/time.h>
#include "Router.h"
#include "SyncInterest.h"
#include "Data.h"
#include "Helper.h"
#include "Logger.h"
#include "Topology.h"

extern int keepRunning;
extern int ENABLE_NBCACHE;

// 构造函数
Router::Router(int ID) {
    //初始化自己的名字
    string router = "r" + to_string(ID);
    this->setName("BUPT", "417", router);

    //初始化端口将由Network来完成

    //初始化CS

    string configFileName = "Data/NetworkInfo/" + router + ".txt";
    cs.init(configFileName, ROUTER_CS_MAX_SIZE);

    //初始化NAME LSDB
    this->initLSDB();

//    this->generateFIB(ENABLE_NBCACHE);

    // 初始化 PIT 相关的锁对象
    pthread_mutex_init(&mutexPit, nullptr);

    // 初始化 rawJobsQue 相关的锁对象
    pthread_mutex_init(&mutexRawJobs, nullptr);
    sem_init(&semRawJobs, 0, 0);

    string csOutFileName = "Data/result/CSTimeStamp/CSTimeStamp_" + name.getDevice() + ".txt";
    csQueOutputFile.open(csOutFileName, std::ios::out);
    if (!csQueOutputFile) {
        Error("无法打开文件" + csOutFileName);
        exit(5);
    }
}


// 析构函数
Router::~Router() {
    // 清理 PIT 相关的锁对象
    pthread_mutex_destroy(&mutexPit);

    // 清理 rawJobsQue 相关的锁对象
    pthread_mutex_destroy(&mutexRawJobs);
    sem_destroy(&semRawJobs);

    if (csQueOutputFile) {
        csQueOutputFile.close();
    }
}


void Router::setName(const string &network, const string &site, const string &router) {
    Router::name.setNetwork(network);
    Router::name.setSite(site);
    Router::name.setDevice(router);
}


const std::vector<Port> &Router::getPorts() const {
    return ports;
}


void Router::addPort(int neighborID, Port &port) {
    int size = ports.size();
    this->neighborToPortLoc.insert(std::pair<int, int>(neighborID, size));
    ports.push_back(port);
}

void *Router::listenPort(void *arg) {
    printlog("listenPort正在运行");

    Arg *rArg = (Arg *) arg;
    int portID = rArg->portID;
    Router *r = rArg->rthis;

    while (keepRunning) {

        Pkt *buf = r->getPorts()[portID].getRLink()->read();

        WaitingQueItem item;
        item.setInPortId(portID);
        item.setPkt(buf);

//        printlog("路由器" + r->name.getDevice() + "的端口" + to_string(portID) + " 收到包 {" + getresourcename(pbr) + "}");
        r->AddPktToRawJobs(item);

        //记录进入路由器缓冲区任务队列的时间
        if ((buf->getType() == INST) || (buf->getType() == DATA)) {
            timeval intv{};
            gettimeofday(&intv, nullptr);
            Logger::getInstance().getRouterTimeLogs()[r->getSelfID()].insertByInTime(buf->getId(), intv);
        }
    }
    printlog(r->name.getDevice() + " listenPort " + to_string(portID) + "thread exits!");
    return (void *) "fun成功执行";
}


//定时发送syncInterest包
void *Router::syncLSDB(void *arg) {
    auto *rthis = (Router *) arg;
    int count = 0;
//    while (keepRunning) {
    for (int k = 0; k < 10; k++) {//静态拓扑下发5次就足够了
        int n = rthis->ports.size();
        for (int i = 0; i < n; i++) {
            NeighborType neighborType = rthis->ports[i].getNeighborType();
            if ((neighborType == ROUTER) || (neighborType == SERVER)) {//向所有的路由器和SERVER邻居端口发送SYNCINST
                string routerOrServer = rthis->NeighborNameOfPortLoc(i);
                NamePrefix name(rthis->name.getNetwork(), rthis->name.getSite(), routerOrServer, "sync");
                int pktid = PKT_NUM * Topology::getInstance().getHostNum() + rthis->getSelfID() * 100 + count;
                SyncInterest *syncInterestp = new SyncInterest(pktid, name, rthis->getSelfID());

                rthis->sendPacket(syncInterestp, rthis->ports[i]);
                count++;
            }
        }
        usleep(200000);//每隔一段时间向各个端口广播syncInterest
    }
    printlog(rthis->name.getDevice() + " syncThread exit!");
    return nullptr;
}


void Router::sendPacket(Pkt *buf, Port &p) {
    p.getWLink()->write(buf);
}

void Router::initLSDB() {

    NameLSA nlsa = this->generateOwnNameLSA(0);

    //在初始的lsdb中加入最初的namelsa,并修改前缀_路由器map
    pthread_mutex_lock(&this->lsdb.mutex);

    this->lsdb.addNameLSA(nlsa);
//    this->lsdb.updateNamePrefixToRouterIDs(lsdb.getNameLSAs()[0]);

    pthread_mutex_unlock(&this->lsdb.mutex);
//    this->lsdb.show();
}


pthread_t Router::run() {
    pthread_t h0thread;
    pthread_create(&h0thread, nullptr, Router::MainThread, this);
    return h0thread;
}

// 路由器启动线程，创建各子线程
void *Router::MainThread(void *arg) {
    auto *rthis = (Router *) arg;

    //////////
    // 创建线程：周期性发 SyncInterest 包
    pthread_t syncThread;
    if (pthread_create(&syncThread, nullptr, syncLSDB, arg) != 0) { Error("syncLSDB线程创建失败"); }
    else { printlog("syncLSDB线程创建成功！pid:" + to_string(getpid()) + " tid:" + to_string(gettid())); }

    //////////
    // 创建线程：为每个端口创建一个收包线程
    uint nPorts = rthis->getPorts().size();

    // 设置线程参数
    Arg *arguments = new Arg[nPorts];
    for (int i = 0; i < nPorts; i++) {
        arguments[i].rthis = rthis;
        arguments[i].portID = i;
    }

    // 为每一个端口创建一个收包线程
    auto *threads = new pthread_t[nPorts];
    for (int i = 0; i < nPorts; i++) {
        if (pthread_create(&threads[i], nullptr, listenPort, &arguments[i]) != 0) {
            Error("端口监听线程创建失败");
        } else {
            printlog("端口监听线程创建成功!pid:" + to_string(getpid()) + " tid:" + to_string(gettid()));
        }
    }

    //////////
    // 创建线程：从 rawJobsQue 取出每一个任务，调用 parse 处理
    pthread_t fetchAndParseThread;
    if (pthread_create(&fetchAndParseThread, nullptr, fetchAndParseFun, arg) != 0) {
        Error("fetchAndParse线程创建失败");
    } else { printlog("fetchAndParse线程创建成功!pid:" + to_string(getpid()) + " tid:" + to_string(gettid())); }

    //////////
    // 创建线程：在 CS 中查找资源
    pthread_t CSThread;
    if (pthread_create(&CSThread, nullptr, CSFun, arg) != 0) { Error("CS线程创建失败"); }
    else { printlog("CS线程创建成功!pid:" + to_string(getpid()) + " tid:" + to_string(gettid())); }

    //////////
    // 等待各线程执行结束（事实上各线程不会结束）
    pthread_join(syncThread, nullptr);
    for (int i = 0; i < nPorts; i++) { pthread_join(threads[i], nullptr); }
    delete[] arguments;
    delete[] threads;
    pthread_join(CSThread, nullptr);
    pthread_join(fetchAndParseThread, nullptr);
    printlog(rthis->name.getDevice() + " main thread exits!");
    return nullptr;
}

void Router::parse(const WaitingQueItem &item) {
    //设置时间
    TimeItem timeItem{};
    timeItem.setPktid(item.getPkt()->getId());
    timeval intv{};
    gettimeofday(&intv, nullptr);
    timeItem.setInTime(intv);

    //分析处理各种类型的包
    switch (item.getPkt()->getType()) {
        case INST: {
            Interest *instp = (Interest *) item.getPkt();

            if (ENABLE_NBCACHE) {//NB-CACHE forwarding strategy
                usleep(0);

                if (this->cs.isBusy(GATEWAY, this->getSelfID())) {
                    instp->setBusy(true);

                    // 合并 PIT 表项
                    pthread_mutex_lock(&mutexPit);
                    bool forward = pit.addPITRecord(instp->getName(), item.getInPortId(), instp->isBusy());
                    pthread_mutex_unlock(&mutexPit);
//                    printlog(name.getDevice() + "recevied INST id:{" + to_string(instp->getId())
//                             + "} resource:" + instp->getName().getRouter() + "/" + instp->getName().getResource() +
//                             "CS is busy!!  forward:"+ to_string(forward));
                    if (forward) {
                        Logger::getInstance().getRouterTimeLogs()[this->getSelfID()].increaseBypassNum();
                        forwardToNext_nbcache(instp);
                    } else {
                        delete instp;
                    }
                } else {
                    Logger::getInstance().getRouterTimeLogs()[this->getSelfID()].increaseCSMatchNum();
                    cs.addItemToQue(item);
                }
            } else {//D-ROUTER forwarding strategy
                if (!instp->isBusy()) {
                    if ((!this->cs.isBusy(GATEWAY, this->getSelfID())) ||
                        (!this->indexTable.find(instp->getName())))// CS不忙或indexTable里没有
                    {
                        Logger::getInstance().getRouterTimeLogs()[this->getSelfID()].increaseCSMatchNum();
                        cs.addItemToQue(item);
                        break;
                    } else {//CS忙且indexTable里有
                        Logger::getInstance().getRouterTimeLogs()[this->getSelfID()].increaseBypassNum();
                        instp->setBusy(true);    // 给 interest 包 busy 位置 true
                    }
                }

                // 合并 PIT 表项
                pthread_mutex_lock(&mutexPit);
                bool forward = pit.addPITRecord(instp->getName(), item.getInPortId(), instp->isBusy());
                pthread_mutex_unlock(&mutexPit);

                // 如果需要转发该interest,查 FIB 转发到下一跳
                if (forward) {
                    forwardToNext(instp);
                } else {
                    delete instp;
                }
            }

            timeval outtv{};
            gettimeofday(&outtv, nullptr);
            Logger::getInstance().getRouterTimeLogs()[this->getSelfID()].completeRecordById(instp->getId(), outtv);
            break;
        }
        case DATA: {
            Data *datap = (Data *) item.getPkt();

//            printlog(this->name.getDevice() + "收到DATA包 id:{" + to_string(datap->getId()) + "}  resource: "
//                     + datap->getName().getRouter() + "/" + datap->getName().getResource() + "  signature为" +
//                     to_string(datap->getSignature()));

            // 在 PIT 中查找
            pthread_mutex_lock(&mutexPit);
            PITRecord pitRecord = pit.findRecord(datap->getName());

            //往来路端口发包
            if (pitRecord.getInPortsIDs().empty()) {
//                printlog("DATA包 id:{" + to_string(datap->getId()) + "}  resource: " + datap->getName().getRouter() +
//                         "/" + datap->getName().getResource() + "的PIT表项为空!");
                //删除PIT表项
                pit.deleteRecordByNamePrefix(datap->getName());
                delete datap;
            } else {
                for (int inPortID: pitRecord.getInPortsIDs()) {
                    Port &port = ports[inPortID];
                    Data *newdatap = new Data(*datap);
                    this->sendPacket(newdatap, port);
//                    printlog(this->name.getDevice() + "发送包到来路端口" + to_string(port.getWLink()->getLinkId()) +
//                             " id:{" + to_string(datap->getId()) + "}  resource: " + datap->getName().getRouter() +
//                             "/" + datap->getName().getResource());
                }
                //删除PIT表项
                pit.deleteRecordByNamePrefix(datap->getName());
                //把Data包扔到CSWaitingQue里排队
                cs.addItemToQue(item);
            }
            pthread_mutex_unlock(&mutexPit);

            timeval outtv{};
            gettimeofday(&outtv, nullptr);
            Logger::getInstance().getRouterTimeLogs()[this->getSelfID()].completeRecordById(datap->getId(), outtv);
            break;
        }
        case SYNC_INST: {
            SyncInterest *syncInterestp = (SyncInterest *) item.getPkt();
            if (syncInterestp->getName().getRouter() == this->name.getDevice()) {
                printlog(this->name.getDevice() + "收到一个SYNC_INST包");

                //发送SYNC_DATA包
                SyncData *syncDatap = this->generateSyncDataPkt(0 - syncInterestp->getId());
                sendPacket(syncDatap, ports[item.getInPortId()]);
            } else {
                printlog(this->name.getDevice() + "收到一个不是发给自己的SYNC_INST包");
            }
            delete syncInterestp;
            break;
        }
        case SYNC_DATA: {
            printlog(name.getDevice() + "  收到一个SYNC_DATA包");
            //更新LSDB
            SyncData *syncDatap = (SyncData *) item.getPkt();

            this->updateLSDBAndIndexTable(*syncDatap);
//            this->lsdb.show();
            //LSDB改变后重新生成FIB表
            this->generateFIB(ENABLE_NBCACHE);
//            this->fib.show();
            delete syncDatap;
            break;
        }
        default: {
//            printlog(name.getDevice()+"收到了其他包");
            break;
        }
    }

    //add parse time record
    timeval outtv{};
    gettimeofday(&outtv, nullptr);
    timeItem.setOutTime(outtv);
    Logger::getInstance().getParseTimeLogs()[this->getSelfID()].add(timeItem);
}

SyncData *Router::generateSyncDataPkt(int pktid) {
    SyncData *syncDatap = new SyncData();
    syncDatap->setId(pktid);

    syncDatap->setName(this->name.getNetwork(), this->name.getSite(), this->name.getDevice(), "sync");
    pthread_mutex_lock(&lsdb.mutex);
    syncDatap->setNameLSAs(this->lsdb.getNameLSAs());
    pthread_mutex_unlock(&lsdb.mutex);

    return syncDatap;
}

void Router::generateFIB(int enable_nbcache) {
    pthread_mutex_lock(&fib.tableMutex);
    pthread_mutex_lock(&lsdb.mutex);
    //先清空原有fib表
    fib.clear();
    //对于每一个nameprefix生成一条转发表项
    lsdb.generateNamePrefixToRouterIDs();
    auto maps = lsdb.getNamePrefixToRouterIDs();
    for (auto &i: maps) {
        NamePrefix np = i.first;
        set<int> routerIDs = i.second;
        int selfID = this->getSelfID();
        if (enable_nbcache == 0) {
            routerIDs.erase(selfID);    //删除掉自己
        } else {//gurantee  inst pkts in nb-cache never turn back way
            for (auto it = routerIDs.begin(); it != routerIDs.end();) {
                if (*it <= selfID) {
                    it = routerIDs.erase(it);
                } else {
                    it++;
                }
            }
        }

        int serverID = Topology::getInstance().getRouterNum();
        pair<NamePrefix, pair<int, int>> fibRecord = calFIBRecord(np, selfID, routerIDs, serverID);
        fib.insertRecord(fibRecord);
    }
    pthread_mutex_unlock(&lsdb.mutex);
    pthread_mutex_unlock(&fib.tableMutex);
}


const int Router::getSelfID() const {
    string router = name.getDevice();
    string routerStr(router.begin() + 1, router.end());
    int routerID = atoi(routerStr.c_str());
    return routerID;
}

pair<NamePrefix, pair<int, int>>
Router::calFIBRecord(const NamePrefix &np, int src, const set<int> &destRouters, int destServer) {
    const int MAXINT = 9999;
    const int vnum = Topology::getInstance().getVertexNum() - Topology::getInstance().getHostNum();
    int prev[vnum];
    int dist[vnum];
    memset(prev, 0, sizeof(int) * vnum);
    memset(dist, 0, sizeof(int) * vnum);

    this->dijkstra(src, prev, dist);

    //挑出去路由器最短的路
    int nextHopIDToRouter;
    if (destRouters.empty()) {
        nextHopIDToRouter = -1;
    } else {
        int min = MAXINT;
        set<int>::iterator minIter;
        for (auto it = destRouters.begin(); it != destRouters.end(); it++) {
            if (dist[(*it)] < min) {
                min = dist[(*it)];
                minIter = it;
            }
        }
        vector<int> pathToRouter = generatePath(src, *minIter, prev);
        nextHopIDToRouter = pathToRouter[1];
    }

    //挑出去服务器的最短的路
    vector<int> pathToServer = generatePath(src, vnum - 1, prev);
    int nextHopIDToServer = pathToServer[1];

    //生成FIB表项
    return make_pair(np, make_pair(nextHopIDToRouter, nextHopIDToServer));
}

void Router::dijkstra(int src, int *prev, int *dist) {
    const int MAXINT = 9999;
    const int vnum = Topology::getInstance().getVertexNum() - Topology::getInstance().getHostNum();//去除掉最后的主机
    bool flag[vnum];

    //初始化
    for (int i = 0; i < vnum; i++) {
        flag[i] = false;
        dist[i] = Topology::getInstance().getEdge(src, i);
        if (dist[i] < MAXINT) {
            prev[i] = src;
        } else {
            prev[i] = -1;
        }
    }
    flag[src] = true;
    dist[src] = 0;

    //每次将一个点确定最短路径
    for (int i = 0; i < vnum; i++) {
        if (i == src) {
            continue;
        } else {
            int min = MAXINT;
            int v;
            for (int j = 0; j < vnum; j++) {
                if (!flag[j] && dist[j] < min) {
                    v = j;
                    min = dist[j];
                }
            }
            flag[v] = true;
            for (int k = 0; k < vnum; k++) {
                if ((!flag[k]) && (dist[k] > (dist[v] + Topology::getInstance().getEdge(v, k)))) {
                    dist[k] = dist[v] + Topology::getInstance().getEdge(v, k);
                    prev[k] = v;
                }
            }
        }
    }
}

vector<int> Router::generatePath(int src, int dest, const int prev[]) {
    vector<int> path;
    int i = dest;
    while (i != src) {
        path.push_back(i);
        i = prev[i];
    }
    path.push_back(i);
    reverse(path.begin(), path.end());
    return path;
}

void *Router::CSFun(void *arg) {
    auto *rthis = (Router *) arg;
    while (keepRunning) {
        bool deleteFlag = false;
        WaitingQueItem item = rthis->cs.getItemFromQue();

        switch (item.getPkt()->getType()) {
            case INST: {
                auto *instp = (Interest *) item.getPkt();
                NamePrefix np = instp->getName();
                CSRecord csRecord = rthis->cs.findRecord(np);

                if (rthis->name.getDevice()[1] == OLD_ROUTER) {
                    usleep(OLD_ROUTER_SLEEP_TIME);
                } else {
                    usleep(ROUTER_SLEEP_TIME);
                }

                //在CS中查找Interest包中请求的名称前缀
                if (!csRecord.getNamePrefix().getRouter().empty()) {//在CS中找到了,发回data包
                    Logger::getInstance().getRouterTimeLogs()[rthis->getSelfID()].increaseCSHitNum();
                    auto datap = new Data(0 - instp->getId(), np, rthis->getSelfID(),
                                          "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
//                    printlog(rthis->name.getDevice() + "csfun发出DATA包:id{" + to_string(datap->getId()) + "} resource:"
//                             + datap->getName().getRouter() + "/" + datap->getName().getResource());

                    Port &port = rthis->ports[item.getInPortId()];
                    rthis->sendPacket(datap, port);
                    deleteFlag = true;
                } else {//在CS中没找到，查FIB转发并添加PIT表项
//                    printlog(rthis->name.getDevice() + "csfun  CS don't find INST :id{" + to_string(instp->getId()) +
//                             "} resource:"
//                             + instp->getName().getRouter() + "/" + instp->getName().getResource());
                    // 合并 PIT 表项
                    pthread_mutex_lock(&rthis->mutexPit);
                    bool forward = rthis->pit.addPITRecord(instp->getName(), item.getInPortId(), false);
                    pthread_mutex_unlock(&rthis->mutexPit);

                    // 查 FIB 转发到下一跳
                    if (forward) {
                        if (ENABLE_NBCACHE) {
                            rthis->forwardToNext_nbcache(instp);
                        } else {
                            rthis->forwardToNext(instp);
                        }
                    } else {
                        deleteFlag = true;
                    }
                }
                break;
            }
            case DATA: {
                timeval t1{};
                gettimeofday(&t1, nullptr);
                Data *datap = (Data *) item.getPkt();

                // 增加 CS 表项
                CSRecord toBeAddedRecord;
                toBeAddedRecord.setNamePrefix(datap->getName());
                toBeAddedRecord.setContent(datap->getContent());

                pthread_mutex_lock(&rthis->cs.recordsMutex);
                int mayDeletedRecordLoc = -1;
                bool suceedToAdd = rthis->cs.addRecord(toBeAddedRecord, mayDeletedRecordLoc); //看看这个data包是不是原来就有

                //修改LSDB并修改map
                if (suceedToAdd) {//如果成功地给CS添加了新表项
                    pthread_mutex_lock(&rthis->lsdb.mutex);

                    //设置LSDB的新的nameLSAS[0]
                    NameLSA newNLSA = rthis->lsdb.getNameLSAs()[0];
                    newNLSA.addNamePrefix(toBeAddedRecord.getNamePrefix());
//
                    if (mayDeletedRecordLoc != -1) {//有要删除的名称前缀
                        newNLSA.deleteNamePrefixByLoc(mayDeletedRecordLoc);

                    }
                    //更新LSDB的nameLSAS[0]
                    newNLSA.increaseVersion();
//                    rthis->lsdb.setNameLSA(0, newNLSA);
                    rthis->lsdb.updateNameLSA(newNLSA);

                    pthread_mutex_unlock(&rthis->lsdb.mutex);
                }

                pthread_mutex_unlock(&rthis->cs.recordsMutex);
                deleteFlag = true;
                break;
            }
            default: {
                printlog("csfun received errorneous pkt!");
                break;
            }
        }

        timeval outtv{};
        gettimeofday(&outtv, nullptr);
        Logger::getInstance().getRouterTimeLogs()[rthis->getSelfID()].completeRecordById(item.getPkt()->getId(),
                                                                                         outtv);
        rthis->outputCSTimestamp(item);//输出时间戳
        if (deleteFlag) {
            delete item.getPkt();
        }
    }
    printlog(rthis->name.getDevice() + " CS thread exits!");
    return nullptr;
}

string Router::NeighborNameOfPortLoc(int loc) {
    string str;
    for (auto &it: this->neighborToPortLoc) {
        if (it.second == loc) {
            int neighborID = it.first;
            if (neighborID < Topology::getInstance().getRouterNum()) {
                str = "r" + to_string(neighborID);
                break;
            }
            if (neighborID < (Topology::getInstance().getRouterNum() + Topology::getInstance().getServerNum())) {
                str = "s" + to_string(neighborID - Topology::getInstance().getRouterNum());
                break;
            }
            str = "h" + to_string(
                    neighborID - (Topology::getInstance().getRouterNum() + Topology::getInstance().getServerNum()));
        }
    }
    return str;
}

NameLSA Router::generateOwnNameLSA(int versionFLag) {
    pthread_mutex_lock(&this->lsdb.mutex);

    NameLSA nlsa;
    nlsa.setNetwork(this->name.getNetwork());
    nlsa.setSite(this->name.getSite());
    nlsa.setRouter(this->name.getDevice());
    nlsa.setNameLsa("NameLSA");
    if (versionFLag == 0) {
        nlsa.setVersion(0);
    } else {
        nlsa.setVersion(this->lsdb.getNameLSAs()[0].getVersion() + 1);//版本号更新
    }

    //拥有的名字在CS中
    pthread_mutex_lock(&this->cs.recordsMutex);

    vector<NamePrefix> nps;
    int n = cs.getRecords().size();
    nps.reserve(n);
    for (int i = 0; i < n; i++) {
        nps.push_back(this->cs.getRecords()[i].getNamePrefix());
    }
    nlsa.setNamePrefixs(nps);

    pthread_mutex_unlock(&this->cs.recordsMutex);

    pthread_mutex_unlock(&this->lsdb.mutex);

    return nlsa;
}

void Router::updateLSDBAndIndexTable(const SyncData &syncData) {
    for (const auto &nlsa: syncData.getNameLSAs()) {
        //如果是remote server发来的NameLSA就更新IndexTable
        if ((nlsa.getRouter()[0] == 's') && (nlsa.getVersion() > this->indexTable.getVersion())) {
            this->indexTable.setVersion(nlsa.getVersion());
            this->indexTable.setNamePrefixs(nlsa.getNamePrefixs());
            break;
//            indexTable.show();
        }

        pthread_mutex_lock(&this->lsdb.mutex);
        //若nlsa在LSDB中，则根据version号更新
        int size = this->lsdb.getNameLSAs().size();
        int i;
        for (i = 0; i < size; i++) {

            NameLSA temp = this->lsdb.getNameLSAs()[i];
            if ((temp.getNetwork() == nlsa.getNetwork()) && (temp.getSite() == nlsa.getSite()) &&
                (temp.getRouter() == nlsa.getRouter()) && (temp.getNameLsa() == nlsa.getNameLsa())) {
                if (temp.getVersion() < nlsa.getVersion()) {
                    lsdb.setNameLSA(i, nlsa);
//                    lsdb.updateNamePrefixToRouterIDs(nlsa);
                }
                break;
            }
        }
        if (i == size) {         //若nlsa不在LSDB中，则在LSDB末尾加入
            if ((this->getSelfID() == 0) && nlsa.getRouter() == "r8") {
                cout << "this way!" << endl;
            }
            this->lsdb.addNameLSA(nlsa);
//            this->lsdb.updateNamePrefixToRouterIDs(nlsa);
        }
        pthread_mutex_unlock(&this->lsdb.mutex);
    }
}

void *Router::fetchAndParseFun(void *arg) {
    auto *rthis = (Router *) arg;

    //一直循环拿包解析包的线程
    while (keepRunning) {
        WaitingQueItem pktItem = rthis->GetPktFromRawJobs();
//        printlog(rthis->name.getDevice() + "从缓冲区里读到包 {" + to_string(pktItem.getPkt()->getId()) + "}");
        rthis->parse(pktItem);
    }
    printlog(rthis->name.getDevice() + " fetchAndParse thread exits!");
    return nullptr;
}


// 查 FIB 转发到下一跳
void Router::forwardToNext(Interest *instp) {
    //查FIB转发到下一跳
    int nextHop;
    auto result_it = this->fib.match(instp->getName());
    if (result_it != fib.getFibMap().end()) {
        if (result_it->second.first < 0) {//这种情况是当前路径已经越过了拥有这个名称内容的路由器，要把兴趣包转发给remote server
            instp->setBusy(true);
            nextHop = Topology::getInstance().getRouterNum();            // 下一跳是向 RemoteServer 走（假定只有一个 RemoteServer）
        } else {
            nextHop = (instp->isBusy()) ? result_it->second.second
                                        : result_it->second.first;    //找到下一跳路由器或server是谁
        }

        int outPortLoc = this->neighborToPortLoc[nextHop]; //找到本路由器的哪一个端口连接这个下一跳
        Port &port = this->ports[outPortLoc];

//        printlog(this->name.getDevice() + "查fib,将此INST包id:{" + to_string(instp->getId()) + "}  resource:"
//                 + instp->getName().getRouter() + "/" + instp->getName().getResource() + "转发给下一跳路由器" +
//                 to_string(nextHop));
        this->sendPacket(instp, port);
    } else {
        printlog(this->name.getDevice() + "查fib,don't have record id:{" + to_string(instp->getId()) +
                 "}    resource: " + instp->getName().getRouter() + "/" + instp->getName().getResource());
        exit(8);
    }
}

void Router::forwardToNext_nbcache(Interest *instp) {
    //查FIB转发到下一跳
    auto result_it = this->fib.match(instp->getName());
    if (result_it != fib.getFibMap().end()) {
        int nextHop = result_it->second.first;
        int outPortLoc = this->neighborToPortLoc[nextHop]; //找到本路由器的哪一个端口连接这个下一跳
        Port &port = this->ports[outPortLoc];

//        printlog(this->name.getDevice() + "查fib,将此INST包id:{" + to_string(instp->getId()) + "}  resource:"
//                 + instp->getName().getRouter() + "/" + instp->getName().getResource() + "转发给下一跳路由器" +
//                 to_string(nextHop));
        this->sendPacket(instp, port);
    } else {
        printlog(this->name.getDevice() + "查fib,don't have record" + "{" + instp->getName().getResource() + "}");
        exit(8);
    }
}


WaitingQueItem Router::GetPktFromRawJobs() {
    sem_wait(&semRawJobs);
    pthread_mutex_lock(&mutexRawJobs);

    WaitingQueItem p = rawJobsQue.front();
    rawJobsQue.pop();

    timeval outtv{};
    gettimeofday(&outtv, nullptr);
    Logger::getInstance().getRawJobsTimeLogs()[this->getSelfID()].completeByPktid(p.getPkt()->getId(), outtv);

    pthread_mutex_unlock(&mutexRawJobs);
    return p;
}


void Router::AddPktToRawJobs(const WaitingQueItem &p) {
    pthread_mutex_lock(&mutexRawJobs);
    rawJobsQue.push(p);

    //记录进任务队列的时间
    timeval intv{};
    gettimeofday(&intv, nullptr);
    Logger::getInstance().getRawJobsTimeLogs()[this->getSelfID()].insertByInTime(intv, p.getPkt()->getId(),
                                                                                 rawJobsQue.size());
    pthread_mutex_unlock(&mutexRawJobs);
    sem_post(&semRawJobs);        // 增加包的时候信号量加一
}

void Router::outputCSTimestamp(WaitingQueItem item) {
    string in = TimevalToString(item.getInTime());
    string out = TimevalToString(item.getOutTime());

    double milsec =
            (item.getOutTime().tv_sec - item.getInTime().tv_sec) * 1000 +
            (item.getOutTime().tv_usec - item.getInTime().tv_usec) / 1000;
    csQueOutputFile << item.getPkt()->getId() << "     "
                    << item.getPkt()->getName().getRouter()
                       + '/' + item.getPkt()->getName().getResource() + "     " +
                       in + "     " + out + "     " << milsec << "     " << item.getQueLen() << endl;
}

bool Router::queueRawJobsIsBusy(int busyBar, const set<int> &exceptRouterIDs, int routerID) {
    if (exceptRouterIDs.find(routerID) != exceptRouterIDs.end()) {//routerID is in exceptRouterIDs
//        printlog("$$$$$$$$"+ to_string(routerID)+"CS  isn't busy");
        return false;
    }

    bool ret;
    pthread_mutex_lock(&mutexRawJobs);
    int len = rawJobsQue.size();
    printlog(this->name.getDevice()+"'s rawJobsQue length is"+ to_string(len) );
    pthread_mutex_unlock(&mutexRawJobs);
    if (len > busyBar) {
        ret = true;
    } else {
        ret = false;
    }
    return ret;
}

int Router::getRawJobsQueLength() {
    pthread_mutex_lock(&mutexRawJobs);
    int len = rawJobsQue.size();
    pthread_mutex_unlock(&mutexRawJobs);
    return len;
}

int Router::getCSLen() {
    int len=cs.getQueLen();
    return len;
}



