//
// Created by csy on 2023/3/29.
//

#include "Pkt.h"

int Pkt::getId() const {
    return id;
}

void Pkt::setId(int id) {
    Pkt::id = id;
}

PktType Pkt::getType() const {
    return type;
}

void Pkt::setType(PktType type) {
    Pkt::type = type;
}

const NamePrefix &Pkt::getName() const {
    return name;
}

void Pkt::setName(const NamePrefix &name) {
    Pkt::name = name;
}

void Pkt::setName(const std::string& network, const std::string& site, const std::string& routerOrServer,
                  const std::string& resource) {
    Pkt::name.setNetwork(network);
    Pkt::name.setSite(site);
    Pkt::name.setRouter(routerOrServer);
    Pkt::name.setResource(resource);
}

int Pkt::getSignature() const {
    return signature;
}

void Pkt::setSignature(int signature) {
    Pkt::signature = signature;
}
