//
// Created by yw80 on 2022/5/15.
//

#ifndef NDN_NEW_TYPEDEF_H
#define NDN_NEW_TYPEDEF_H
#include <set>
extern int ENABLE_NBCACHE;
extern int BUSY_THRESHOLD;			// 阈值
extern int PKT_NUM;					// 发包数量

extern std::set<int> GATEWAY;
extern char OLD_ROUTER;				// 指定旧的、速度不同的路由器
extern int OLD_ROUTER_SLEEP_TIME;	// 服务器延迟时间（微秒）
extern int ROUTER_SLEEP_TIME;		// 路由器延迟时间（微秒）
extern int SERVER_SLEEP_TIME;		// 服务器延迟时间（微秒）

extern int FLOW_RATE;
extern int LINK_SLEEP_TIME_UNIT;	// 链路延时

extern int ROUTER_CS_MAX_SIZE;		// Router CS 的最大容量
extern int SERVER_CS_MAX_SIZE;		// Server CS 的最大容量


// 路由器端口连接的邻居类型：NONE 表示该端口还没连接任何的链路
enum NeighborType{
	NONE=0, ROUTER, HOST, SERVER
};

#endif //NDN_NEW_TYPEDEF_H
