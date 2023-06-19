//
// Created by suggie on 2022/2/6.
//

#include "Port.h"

Port::Port() {
    this->neighborType=NONE;
    this->rLink = nullptr;
    this->wLink = nullptr;
}

Link *Port::getWLink() const {
    return wLink;
}

void Port::setWLink(Link *wLink) {
    Port::wLink = wLink;
}

Link *Port::getRLink() const {
    return rLink;
}

void Port::setRLink(Link *rLink) {
    Port::rLink = rLink;
}

NeighborType Port::getNeighborType() const {
    return neighborType;
}

void Port::setNeighborType(NeighborType neighborType) {
    Port::neighborType = neighborType;
}
