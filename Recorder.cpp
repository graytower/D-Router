//
// Created by csy on 2023/5/14.
//
#include <fstream>
#include <unistd.h>
#include <sys/time.h>
#include "Helper.h"
#include "Recorder.h"
#include "Topology.h"

extern timeval startTime;

pthread_t Recorder::run(Network *network) {
    pthread_t loggerThread;
    pthread_create(&loggerThread, nullptr, Recorder::mainThread, network);
    return loggerThread;
}

void *Recorder::mainThread(void *arg) {
    sleep(5);

    auto *network = (Network *) arg;

    std::fstream fs[Topology::getInstance().getRouterNum()];
    for (int i = 0; i < Topology::getInstance().getRouterNum(); i++) {
        fs[i].open("Data/result/queLength/queLen_r" + to_string(i) + "_thre" + to_string(BUSY_THRESHOLD)
                   + "_enable" + to_string(ENABLE_NBCACHE), ios::out);
        if (!fs[i]) {
            Error("can not open file Data/result/queLength/queLen_r" + to_string(i) + "_thre" +
                  to_string(BUSY_THRESHOLD)
                  + "_enable" + to_string(ENABLE_NBCACHE));
        }
    }
    while (true) {
        timeval tv{};
        gettimeofday(&tv, nullptr);
        double difference = calTimeDif(tv, startTime);
        for (int i = 0; i < Topology::getInstance().getRouterNum(); i++) {
            int rlen = network->getRouters()[i]->getRawJobsQueLength();
            int cslen = network->getRouters()[i]->getCSLen();
            fs[i] << difference << "     " << rlen << "      " << cslen << "      " << rlen + cslen << endl;
        }
        usleep(1000);
    }
}

