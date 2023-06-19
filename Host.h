//
// Created by suggie on 2022/2/20.
//

#ifndef NDN_NEW_HOST_H
#define NDN_NEW_HOST_H

#include <string>
#include "Port.h"
#include<cstring>
#include "Interest.h"
#include "SentInterestTable.h"
#include "Logger.h"


class Host {
private:
    std::string network;
    std::string site;
    std::string host;
    Port port;
    SentInterestTable sit;

public:
	explicit Host(int hostID);

public:
	pthread_t run();
private:
	static void* MainThread(void *arg);

public:
    const std::string &getNetwork() const;

    void setNetwork(const std::string &network);

    const std::string &getSite() const;

    void setSite(const std::string &site);

    const std::string &getHost() const;

    void setHost(const std::string &host);

    const Port &getPort() const;

    void setPort(const Port &port);

    void sendInterest(Interest* inst);

    void sendPkts(std::string fileName,int interval);

    static void * recvAndParsePkts(void *arg);

    int getSelfDeviceID();

};


#endif //NDN_NEW_HOST_H
