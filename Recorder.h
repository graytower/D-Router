//
// Created by csy on 2023/5/14.
//

#ifndef NDN_NEW_RECORDER_H
#define NDN_NEW_RECORDER_H


#include <csignal>
#include "Network.h"

class Recorder {

public:
    pthread_t run(Network *network);
private:
    static void *mainThread(void *arg);
};


#endif //NDN_NEW_RECORDER_H
