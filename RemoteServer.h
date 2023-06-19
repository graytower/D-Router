//
// Created by suggie on 2022/3/1.
//

#ifndef NDN_NEW_REMOTESERVER_H
#define NDN_NEW_REMOTESERVER_H

#include <iostream>
#include <string>
#include "Port.h"
#include "CS.h"
#include "SyncData.h"

class RemoteServer {
private:
    std::string network;
    std::string site;
    std::string server;
    Port port;
    CS cs;

public:
    RemoteServer();

	pthread_t run();
private:
	static void* MainThread(void *arg);

public:
    void sendPacket(Pkt *buf);

    void recvAndParsePacket();

    const std::string &getNetwork() const;

    void setNetwork(const std::string &network);

    const std::string &getSite() const;

    void setSite(const std::string &site);

    const Port &getPort() const;

    void setPort(const Port &port);

    SyncData *generateSyncDataPkt(int pktid);

    int getSelfID();
};



#endif //NDN_NEW_REMOTESERVER_H
