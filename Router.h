//
// Created by suggie on 2022/2/4.
//

#ifndef NDN_NEW_ROUTER_H
#define NDN_NEW_ROUTER_H

#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include "DeviceName.h"
#include "Port.h"
#include "NamePrefix.h"
#include "CS.h"
#include "PIT.h"
#include "LSDB.h"
#include "SyncData.h"
#include "FIB.h"
#include "IndexTable.h"
#include "TimePair.h"

class Router {
private:
    DeviceName name;
    std::vector<Port> ports;                // 通过这两个结构知道自己的邻居
    std::map<int, int> neighborToPortLoc;   //neighborID 和端口号
    LSDB lsdb;      //NAME LSDB   知道每个名字在谁那，存的是一个个NAME LSA
    IndexTable indexTable;//索引表，用来知道SERVER里有什么名字
    CS cs;      //自己有什么名字
    PIT pit;
    FIB fib;        //NAME LSDB改变一次就要算过一次
	std::queue<WaitingQueItem>  rawJobsQue;			// 路由器收到的原生数据包
    pthread_mutex_t         	mutexPit;               // 操作 pit 对象的互斥锁
	pthread_mutex_t				mutexRawJobs;			// 操作 rawJobsQue 队列需要用到的互斥锁
	sem_t						semRawJobs;				// 操作 rawJobsQue 队列需要用到的同步锁
    std::fstream csQueOutputFile;

public:
    explicit Router(int ID);

	~Router();

	pthread_t run();
private:
	static void* MainThread(void *arg);

public:

    int getCSLen();

    int getRawJobsQueLength();

    string NeighborNameOfPortLoc(int loc);

    //周期性地向所有端口广播syncInterest
    static void *syncLSDB(void *arg);

    // 监听端口，将收到的包放入 rawJobsQue
    static void *listenPort(void *arg);

    static void *CSFun(void *arg);    //解析包

    static void *fetchAndParseFun(void *arg); //拿包解析包线程

    bool queueRawJobsIsBusy(int busyBar,const set<int> &exceptRouterIDs, int routerID);

    //在从指定端口发送指定包
    void sendPacket(Pkt* buf, Port &p);

    void parse(const WaitingQueItem &item);

    //初始化LSDB。将自己最初拥有的名称前缀构成的NameLSA包放在LSDB中
    void initLSDB();

    SyncData * generateSyncDataPkt(int pktid);

    pair<NamePrefix, pair<int, int>> calFIBRecord(const NamePrefix &np, int src, const set<int> &destRouters, int destServer);

    void generateFIB(int enable_nbcache);

    NameLSA generateOwnNameLSA(int versionFLag);

    void updateLSDBAndIndexTable(const SyncData &syncData);

    void forwardToNext(Interest *instp);

    void forwardToNext_nbcache(Interest *instp);

    WaitingQueItem GetPktFromRawJobs();				// 从 rawJobsQue 队列中获取一个数据包

    void AddPktToRawJobs(const WaitingQueItem &p);		// 向 rawJobsQue 队列中添加一个数据包

    const int getSelfID() const;

    void setName(const string &network, const string &site, const string &router);

    void addPort(int neighborID, Port &port);

    const std::vector<Port> &getPorts() const;



private:
    void dijkstra(int src, int *prev, int *dist);

    static vector<int> generatePath(int src, int dest, const int prev[]);

    void outputCSTimestamp(WaitingQueItem item);

};

struct Arg {
    Router *rthis;
    int portID;
};
#endif //NDN_NEW_ROUTER_H
