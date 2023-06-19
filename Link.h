//
// Created by suggie on 2022/1/21.
//

#ifndef NDN_NEW_LINK_H
#define NDN_NEW_LINK_H

#include <semaphore.h>
#include <queue>
#include "Pkt.h"

class Link {
private:
    int linkID;
    int weight;
    std::queue<Pkt*> pkts;

public:
    pthread_mutex_t mutex;
    sem_t full, empty;

    Link(int id, int newWeight);

    ~Link();

    int getLinkId() const;

	void write(Pkt* buf);

    Pkt *read();

};


#endif //NDN_NEW_LINK_H
