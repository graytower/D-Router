//
// Created by suggie on 2021/11/4.
//

#ifndef NDNROUTER_TOPOLOGY_H
#define NDNROUTER_TOPOLOGY_H

#include <string>

using namespace std;

class Topology
{
	// 实现 Singleton 模式
private:
	Topology();
	~Topology();
//public:
    Topology(const Topology &) = delete;
    Topology(const Topology &&) = delete;//关闭拷贝构造函数，右值拷贝构造函数。
    Topology &operator=(const Topology &) = delete;// 关闭赋值运算符重载函数。

public:
    static Topology& getInstance();// 成员变量
private:
	int *data;		// 邻接矩阵
	int n;			// 顶点数
    int m_routerNum;
    int m_hostNum;
    int m_serverNum;

	// 成员函数
private:
	int init(string fileName);

public:
    int  getRouterNum() const		{	return m_routerNum;		}
    void setRouterNum(int routerNum){	m_routerNum = routerNum;}

    int  getHostNum() const			{	return m_hostNum;		}
    void setHostNum(int hostNum)	{	m_hostNum = hostNum;	}

    int  getServerNum() const		{	return m_serverNum;		}
    void setServerNum(int serverNum){	m_serverNum = serverNum;}

    int  getVertexNum() const		{	return n;				}

	int getEdge(int i, int j) const;
	bool setEdge(int i, int j, int value);

	void show() const;
};

#endif //NDNROUTER_TOPOLOGY_H
