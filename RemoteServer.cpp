//
// Created by suggie on 2022/3/1.
//

#include <fstream>
#include <set>
#include <unistd.h>
#include <sys/time.h>
#include "RemoteServer.h"
#include "Interest.h"
#include "ConfigFileInStream.h"
#include "Data.h"
#include "SyncData.h"
#include "SyncInterest.h"
#include "Logger.h"
#include "Helper.h"
#include "Topology.h"


extern int keepRunning;

const std::string &RemoteServer::getNetwork() const {
    return network;
}

void RemoteServer::setNetwork(const std::string &network) {
    RemoteServer::network = network;
}

const std::string &RemoteServer::getSite() const {
    return site;
}

void RemoteServer::setSite(const std::string &site) {
    RemoteServer::site = site;
}


const Port &RemoteServer::getPort() const {
    return port;
}

void RemoteServer::setPort(const Port &port) {
    RemoteServer::port = port;
}

RemoteServer::RemoteServer() {
    this->network = "BUPT";
    this->site = "417";
    this->server = "s0";
    cs.init("Data/NetworkInfo/s0.txt", SERVER_CS_MAX_SIZE);
}

void RemoteServer::sendPacket(Pkt *buf) {
    port.getWLink()->write(buf);
}

void RemoteServer::recvAndParsePacket() {

    Pkt* pkt=port.getRLink()->read();//读链路取包

    timeval inTV{.tv_sec=0, .tv_usec=0};
    gettimeofday(&inTV, nullptr);

    switch (pkt->getType()) {
        case INST: {
           auto instp=(Interest *)pkt;
//            inst.show();

            NamePrefix np = instp->getName();
//            printlog(this->server + "收到一个INST包{" + np.getResource() + "}");
            CSRecord csRecord;
            //在CS中查找Interest包中请求的名称前缀
            usleep(SERVER_SLEEP_TIME);
            csRecord = this->cs.findRecord(np);
            if (!csRecord.getNamePrefix().getRouter().empty()) {//在CS中找到了,发回data包

                Data* datap  =new Data(0-instp->getId(),np, this->getSelfID(),csRecord.getContent());
                sendPacket(datap);

            } else {
                printlog("remote server中没有这个名称前缀，错误的Interest包,丢包！");
            }
            timeval outTV{.tv_sec=0, .tv_usec=0};
            gettimeofday(&outTV, nullptr);

            Logger::getInstance().getServerTimeLog().insert(instp->getId(),inTV,outTV);
            delete instp;
            break;
        }
        case SYNC_INST: {
            SyncInterest * syncInterestp=(SyncInterest *)pkt;
            if (syncInterestp->getName().getRouter() == this->server) {
                printlog(this->server + "收到一个SYNC_INST包");
                //发送SYNC_DATA包
                SyncData* syncDatap = this->generateSyncDataPkt(syncInterestp->getId());

                sendPacket(syncDatap);
            } else {
                printlog(syncInterestp->getName().getRouter() + this->server + "收到一个不是发给自己的SYNC_INST包");
            }
            delete syncInterestp;
            break;
        }
        default: {
            printlog("remote server收到了其他包，忽略。");
            break;
        }
    }

}


pthread_t RemoteServer::run()
{
	pthread_t h0thread;
	pthread_create(&h0thread, nullptr, RemoteServer::MainThread, this);
	return h0thread;
}

void *RemoteServer::MainThread(void *arg) {
    auto sthis = (RemoteServer *) arg;

    //一直收包
    while (keepRunning) {
        sthis->recvAndParsePacket();
    }
    printlog("remote server main thread exits!");
    return nullptr;
}


SyncData* RemoteServer::generateSyncDataPkt(int pktid) {
    SyncData * syncDatap=new SyncData();
    syncDatap->setId(pktid);
    syncDatap->setName(this->network,this->site,this->server,"sync");

    NameLSA nameLsa;
    nameLsa.setNetwork(this->network);

    nameLsa.setSite(this->site);
    nameLsa.setRouter(this->server);
    nameLsa.setNameLsa("NameLSA");
//    nameLsa.setVersion(nameLsa.getVersion()+1);
    nameLsa.setVersion(1);
    nameLsa.setNamePrefixs(this->cs.getNamePrefixs());

    syncDatap->addNameLSA(nameLsa);
    return syncDatap;
}

int RemoteServer::getSelfID() {
    return Topology::getInstance().getRouterNum();//当前的情况是只有一个remote server
}
