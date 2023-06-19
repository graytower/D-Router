//
// Created by suggie on 2022/2/10.
//
#include <sys/wait.h>
#include <sys/shm.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "Network.h"
#include "Topology.h"
#include "Helper.h"

extern sem_t g_sem;

Network::~Network() {

	for(auto & h : hosts){
		delete h;
	}
    for(auto &r:routers){
        delete r;
    }
	delete  server;
    delete []links;
}


void Network::initNetwork() {
	initLinks();

	//初始化路由器
	initRouters();

	//初始化主机
	initHosts();

	//初始化远端服务器
	initRemoteServer();
}

void Network::start() {

    for(int i=0;i<Topology::getInstance().getRouterNum();i++){
        routers[i]->run();
    }


	pthread_t s0thread = server->run();

	pthread_t h0thread = hosts[0]->run();
	pthread_t h1thread = hosts[1]->run();


//	// 等待子线程执行结束
//	pthread_join(r0thread, nullptr);
//	pthread_join(r1thread, nullptr);
//	pthread_join(r2thread, nullptr);
//	pthread_join(r3thread, nullptr);
//	pthread_join(s0thread, nullptr);
//	pthread_join(h0thread, nullptr);
//	pthread_join(h1thread, nullptr);
//	pthread_join(h2thread, nullptr);
//	pthread_join(h3thread, nullptr);
}


void Network::initLinks() {
    int n = Topology::getInstance().getVertexNum();
    links = new Link *[n * n];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
			int edge = Topology::getInstance().getEdge(i, j);
            if ((edge != 9999) && (edge != 0)) {
                string strid = to_string(i) + to_string(j);
                int linkID = atoi(strid.c_str());
                links[i * n + j] = new Link(linkID, edge);
                printf("i:%d  j:%d  linkID:%d  links[i][j]:%p\n", i, j, links[i * n + j]->getLinkId(), links[i * n + j]);
            }
        }
    }
}



void Network::initRouters() {

	int rn = Topology::getInstance().getRouterNum();
	int sn = Topology::getInstance().getServerNum();
	int n = Topology::getInstance().getVertexNum();

    for (int id = 0; id < rn; id++) {//对于每一个路由器
        //初始化名字
        auto* r = new Router(id);
        //初始化端口
        for (int i = 0; i < rn; i++) {//可能连接的每一条链路
			int edge = Topology::getInstance().getEdge(id, i);
            if ((edge != 9999) && (edge != 0)) {
                Port p;
                p.setWLink(links[id * n + i]);
                p.setRLink(links[i * n + id]);
                p.setNeighborType(ROUTER);
                r->addPort(i, p);
                printf("id:%d   i:%d  linksiid:%p   rlink:%p  neighborType:%d \n", id, i, links[i * n + id], p.getRLink(),
                       p.getNeighborType());
            }
        }
        for (int i = rn; i < rn + sn; i++) {
			int edge = Topology::getInstance().getEdge(id, i);
            if ((edge != 9999) && (edge != 0)) {
                Port p;
                p.setWLink(links[id * n + i]);
                p.setRLink(links[i * n + id]);
                p.setNeighborType(SERVER);
                r->addPort(i, p);
                printf("id:%d   i:%d  linksiid:%p   rlink:%p  neighborType:%d \n", id, i, links[i * n + id], p.getRLink(),
                       p.getNeighborType());
            }
        }
        for (int i = rn + sn; i < n; i++) {
			int edge = Topology::getInstance().getEdge(id, i);
            if ((edge != 9999) && (edge != 0)) {
                Port p;
                p.setWLink(links[id * n + i]);
                p.setRLink(links[i * n + id]);
                p.setNeighborType(HOST);
                r->addPort(i, p);
                printf("id:%d   i:%d  linksiid:%p   rlink:%p  neighborType:%d \n", id, i, links[i * n + id], p.getRLink(),
                       p.getNeighborType());
            }
        }
        this->routers.push_back(r);
    }
}

const vector <Router*> &Network::getRouters() const {
    return routers;
}

void Network::setRouters(const vector <Router*> &routers) {
    Network::routers = routers;
}



void Network::initRemoteServer() {
    server = new RemoteServer();
	int serverID = Topology::getInstance().getRouterNum();
    //设置端口
	int n = Topology::getInstance().getVertexNum();
    for (int i = 0; i < n; i++) {
		int edge = Topology::getInstance().getEdge(serverID, i);
        if ((edge != 9999) && (edge != 0)) {
            Port p;
            p.setWLink(links[serverID * n + i]);
            p.setRLink(links[i * n + serverID]);
            this->server->setPort(p);
            break;
        }
    }
}


RemoteServer* Network::getServer() const {
    return server;
}



void Network::initHosts() {
    //对于每一个主机
	for(int hostID = Topology::getInstance().getRouterNum() + Topology::getInstance().getServerNum(); hostID<Topology::getInstance().getVertexNum(); hostID++){
        //设置端口
        int n = Topology::getInstance().getVertexNum();
        for (int i = 0; i < n; i++) {
			int edge = Topology::getInstance().getEdge(hostID, i);
            if ((edge != 9999) && (edge != 0)) {
                Port p;
                p.setWLink(links[hostID * n + i]);
                p.setRLink(links[i * n + hostID]);
                p.setNeighborType(ROUTER);
				Host* hostp = new Host(hostID - Topology::getInstance().getRouterNum() - Topology::getInstance().getServerNum());
                hostp->setPort(p);
                hosts.push_back(hostp);
                break;
            }
        }
    }
}

const vector<Host *> &Network::getHosts() const {
    return hosts;
}

Network::Network() {
    links= nullptr;
    server= nullptr;
}



