//
// Created by suggie on 2021/11/23.
//

#ifndef NDNROUTER_ROUTERNAME_H
#define NDNROUTER_ROUTERNAME_H


#include <string>

class DeviceName {
private:
    std::string network;
    std::string site;
    std::string device;

public:
    const std::string &getNetwork() const {
        return network;
    }

    void setNetwork(const std::string &network) {
        DeviceName::network = network;
    }

    const std::string &getSite() const {
        return site;
    }

    void setSite(const std::string &site) {
        DeviceName::site = site;
    }

    const std::string &getDevice() const {
        return device;
    }

    void setDevice(const std::string &device) {
        DeviceName::device = device;
    }

};


#endif //NDNROUTER_ROUTERNAME_H
