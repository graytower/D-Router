//
// Created by suggie on 2022/2/10.
//

#ifndef NDN_NEW_NETWORK_H
#define NDN_NEW_NETWORK_H


#include <vector>
#include "Link.h"
#include "Router.h"
#include "Host.h"
#include "RemoteServer.h"

using namespace std;

class Network {
private:
    Link** links;
    vector<Router*> routers;
    vector<Host*> hosts;
    RemoteServer* server;

public:
	Network();
	~Network();

    void initNetwork();

	void start();

    void  initLinks();

    void initRouters();

    void initHosts();

    void initRemoteServer();

    const vector <Router*> &getRouters() const;

    void setRouters(const vector <Router*> &routers);

    RemoteServer* getServer() const;

    const vector<Host *> &getHosts() const;

};


#endif //NDN_NEW_NETWORK_H
