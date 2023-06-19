#include "typedef.h"
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include "Network.h"
#include "Helper.h"
#include "Topology.h"
#include "Recorder.h"

using namespace std;

int keepRunning = 1;
sem_t g_sem;//用于接收子线程的信号，从主线程结束整个程序

int main(int argc, char *argv[]) {
    int res = sem_init(&g_sem, 1, 0);
    if (res != 0)
    {
        printlog("g_sem initialization failed");
    }
    printlog("g_sem init");

    printlog("#main pid:"+ to_string(getpid())+" tid:"+ to_string(gettid()));


	if (argc > 1)
	{
		for (int i = 1; i < argc; i += 2)
		{
            if      (strcmp(argv[i], "-ENABLE_NBCACHE") == 0)		ENABLE_NBCACHE  	 = atoi(argv[i + 1]);
			else if (strcmp(argv[i], "-BUSY_THRESHOLD") == 0)		BUSY_THRESHOLD  	 = atoi(argv[i + 1]);
			else if (strcmp(argv[i], "-PKT_NUM") == 0)				PKT_NUM 			 = atoi(argv[i + 1]);
			else if (strcmp(argv[i], "-ROUTER_SLEEP_TIME") == 0)	ROUTER_SLEEP_TIME 	 = atoi(argv[i + 1]);
			else if (strcmp(argv[i], "-SERVER_SLEEP_TIME") == 0)	SERVER_SLEEP_TIME 	 = atoi(argv[i + 1]);
//			else if (strcmp(argv[i], "-HOST_SLEEP_TIME") == 0)		HOST_SLEEP_TIME 	 = atoi(argv[i + 1]);
            else if (strcmp(argv[i], "-FLOW_RATE") == 0)            FLOW_RATE            = atoi(argv[i + 1]);
			else if (strcmp(argv[i], "-LINK_SLEEP_TIME_UNIT") == 0)	LINK_SLEEP_TIME_UNIT = atoi(argv[i + 1]);
//			else if (strcmp(argv[i], "-ROUTER_CS_MAX_SIZE") == 0)	ROUTER_CS_MAX_SIZE	 = atoi(argv[i + 1]);
			else if (strcmp(argv[i], "-SERVER_CS_MAX_SIZE") == 0)	SERVER_CS_MAX_SIZE	 = atoi(argv[i + 1]);
			else if (strcmp(argv[i], "-OLD_ROUTER") == 0)			OLD_ROUTER			 = argv[i + 1][0];
			else if (strcmp(argv[i], "-OLD_ROUTER_SLEEP_TIME") == 0)OLD_ROUTER_SLEEP_TIME= atoi(argv[i + 1]);
		}
	}

    printlog("ENABLE_NBCACHE: "		 + to_string(ENABLE_NBCACHE));
	printlog("BUSY_THRESHOLD: "		 + to_string(BUSY_THRESHOLD));
	printlog("PKT_NUM: "			     + to_string(PKT_NUM));
//	printlog("ROUTER_SLEEP_TIME: "	 + to_string(ROUTER_SLEEP_TIME));
	printlog("SERVER_SLEEP_TIME: "	 + to_string(SERVER_SLEEP_TIME));
//	printlog("HOST_SLEEP_TIME: "	     + to_string(HOST_SLEEP_TIME));
    printlog("FLOW_RATE: "            + to_string(FLOW_RATE));
	printlog("LINK_SLEEP_TIME_UNIT: " + to_string(LINK_SLEEP_TIME_UNIT));
	printlog("ROUTER_CS_MAX_SIZE: "	 + to_string(ROUTER_CS_MAX_SIZE));
	printlog("SERVER_CS_MAX_SIZE: "	 + to_string(SERVER_CS_MAX_SIZE));
	printlog("OLD_ROUTER: "	 		 + string(1, OLD_ROUTER));
	printlog("OLD_ROUTER_SLEEP_TIME: " + to_string(OLD_ROUTER_SLEEP_TIME));

    Network network;
    network.initNetwork();
    network.start();
    Recorder recorder;
    recorder.run(&network);

    sem_wait(&g_sem);
    printlog("从main主线程退出");

//    sleep();
    Logger::getInstance().show();
    Logger::getInstance().output();

    return EXIT_SUCCESS;
}
