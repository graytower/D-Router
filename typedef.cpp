#include <set>

int BUSY_THRESHOLD = 100000;                 // 阈值
int ENABLE_NBCACHE = 1;

int PKT_NUM = 10000;                     // 每个主机发包数量

char OLD_ROUTER = 'x';                   // 指定旧路由器
int OLD_ROUTER_SLEEP_TIME = 1000;        // 路由器延迟时间（微秒）
int ROUTER_SLEEP_TIME = 150;             // 路由器延迟时间（微秒）
int SERVER_SLEEP_TIME = 600;             // 服务器延迟时间（微秒）
//int HOST_SLEEP_TIME = 50;              // HOST 发包延时（微秒）
int FLOW_RATE = 6 * 1000;                // send packets pps;
int LINK_SLEEP_TIME_UNIT = 50;           // 链路延时
std::set<int> GATEWAY = {8, 9};
int ROUTER_CS_MAX_SIZE = 500;            // Router CS 的最大容量
int SERVER_CS_MAX_SIZE = 1000;           // Server CS 的最大容量