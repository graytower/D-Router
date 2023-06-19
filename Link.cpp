//
// Created by suggie on 2022/1/21.
//
#include <sys/shm.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include "Link.h"
#include "Helper.h"
#include "typedef.h"

using namespace std;


void Link::write(Pkt* buf)
{
    sem_wait(&empty);

    pthread_mutex_lock(&mutex);
    pkts.push(buf);
    pthread_mutex_unlock(&mutex);

    sem_post(&full);

    int value;
    sem_getvalue(&full, &value);
//    printlog("link" + to_string(this->linkID) + "write完的full:" + to_string(value));
}


Pkt* Link::read()
{
    Pkt* pktp= nullptr;
    sem_wait(&full);

    pthread_mutex_lock(&mutex);
    pktp=pkts.front();
    pkts.pop();

    usleep(this->weight*LINK_SLEEP_TIME_UNIT);
    pthread_mutex_unlock(&mutex);

    sem_post(&empty);
    return pktp;
}


Link::Link(int id, int newWeight) {
    printlog("Link::Link");
    linkID = id;
    this->weight=newWeight;

    //初始化信号量
    pthread_mutex_init(&mutex, nullptr);
    sem_init(&empty, 0, 2000);//pshared==1表示在进程间共享，初值为2000,表示每个共享内存里最多放2000个包
    sem_init(&full, 0, 0);
}


Link::~Link() {
    printlog("Link::~Link");
    pthread_mutex_destroy(&this->mutex);
    sem_destroy(&this->empty);
    sem_destroy(&this->full);
}


int Link::getLinkId() const {
    return linkID;
}

