//
// Created by suggie on 2022/2/6.
//

#ifndef NDN_NEW_PORT_H
#define NDN_NEW_PORT_H


#include "Link.h"
#include "typedef.h"

class Port {
private:
    NeighborType neighborType;
    Link *wLink;
    Link *rLink;

public:
    Port();

    Link *getWLink() const;

    void setWLink(Link *wLink);

    Link *getRLink() const;

    void setRLink(Link *rLink);

    NeighborType getNeighborType() const;

    void setNeighborType(NeighborType neighborType);

};


#endif //NDN_NEW_PORT_H
