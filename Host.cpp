//
// Created by suggie on 2022/2/20.
//
#include <utility>
#include<vector>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>
#include <csignal>
#include "Host.h"
#include "ConfigFileInStream.h"
#include "Data.h"
#include "Helper.h"
#include "typedef.h"
#include "Topology.h"


extern int keepRunning;
timeval startTime;

const std::string &Host::getNetwork() const {
    return network;
}

void Host::setNetwork(const std::string &network) {
    Host::network = network;
}

const std::string &Host::getSite() const {
    return site;
}

void Host::setSite(const std::string &site) {
    Host::site = site;
}

const std::string &Host::getHost() const {
    return host;
}

void Host::setHost(const std::string &host) {
    Host::host = host;
}

const Port &Host::getPort() const {
    return port;
}

void Host::setPort(const Port &port) {
    Host::port = port;
}

void Host::sendInterest(Interest *inst) {
    port.getWLink()->write(inst);
}

// 发包主线程
void Host::sendPkts(std::string fileName, int interval) {
    sleep(5);//给一点让各个路由器的LSDB同步的时间
    gettimeofday(&startTime, nullptr);
    std::vector<NamePrefix> nps = ConfigFileInStream::readInNamePrefixs(fileName);    //读入文件中的名称前缀


    int pktnum = 0;
    Logger::getInstance().addTotalNum(PKT_NUM);

    int multiple = (this->host[1] - '0') * PKT_NUM;
    string s_multiple = to_string(multiple);
    printlog("$$$$multiple:" + s_multiple);

    while (true) {
        //将名称前缀装入Interest包并发送
        for (auto &np: nps) {
            Interest *instp = new Interest(pktnum + multiple, false, np, this->getSelfDeviceID());

//            inst.show();
            //设置好record
            SentInterestRecord record;
            record.setPktid(pktnum + multiple);
            record.setNamePrefix(np);
            timeval sendTime{};
            gettimeofday(&sendTime, nullptr);
            record.setSendTime(sendTime);
            //给sit加入这条record
            sit.addRecord(record);
//          sit.show();
//			printlog("host发送包id: {" + to_string(instp->getId()) + "},  request:"+instp->getName().getRouter()+"/"+instp->getName().getResource());
            sendInterest(instp);

//            usleep(HOST_SLEEP_TIME);
            usleep(1000000 / FLOW_RATE);
            pktnum++;
            if (pktnum == PKT_NUM) {
                return;
            }
        }
    }
}

// 收包线程
void *Host::recvAndParsePkts(void *arg) {
    auto hthis = (Host *) arg;
    while (keepRunning) {
        Pkt *pkt = hthis->port.getRLink()->read();

        switch (pkt->getType()) {
            case DATA: {
                timeval backTime{.tv_sec=0, .tv_usec=0};
                gettimeofday(&backTime, nullptr);

                Data *datap = (Data *) pkt;
//                data.show();
//                printlog(hthis->host + "收到了一个DATA包 id:{" + to_string(datap->getId()) + "}  resource:" +
//                         datap->getName().getRouter() + "/" + datap->getName().getResource() + "  signature:" +
//                         to_string(datap->getSignature()));

                //删掉sit中所有关于这个名称前缀的请求的表项
                NamePrefix np = datap->getName();
                hthis->sit.matchAndDelete(np, backTime, datap->getSignature());
                delete datap;
                break;
            }
            default: {
//                printlog(hthis->host + "收到了非DATA包，丢掉！");
                break;
            }
        }

    }
    return nullptr;
}

pthread_t Host::run() {
    pthread_t h0thread;
    pthread_create(&h0thread, nullptr, Host::MainThread, this);
    return h0thread;
}

void *Host::MainThread(void *arg) {
    auto hthis = (Host *) arg;

    //收包
    pthread_t recvThread;
    pthread_create(&recvThread, nullptr, recvAndParsePkts, arg);

    //周期性发Interest
//    hthis->sendPkts("Data/request/request_"+hthis->getHost()+".txt", FLOW_RATE);
    hthis->sendPkts("Data/request/request.txt", FLOW_RATE);

    pthread_join(recvThread, nullptr);
    printlog("host main thread exits!");
    return nullptr;
}

Host::Host(int hostID) {
    this->network = "BUPT";
    this->site = "417";
    this->host = "h" + to_string(hostID);
}

int Host::getSelfDeviceID() {
    int selfid =
            Topology::getInstance().getRouterNum() + Topology::getInstance().getServerNum() + (this->host[1] - '0');
    return selfid;
}



