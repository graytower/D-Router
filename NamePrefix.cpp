//
// Created by suggie on 2022/2/19.
//

#include <iostream>
#include "NamePrefix.h"
#include "Helper.h"


const std::string &NamePrefix::getNetwork() const {
    return network;
}

void NamePrefix::setNetwork(const std::string &network) {
    NamePrefix::network = network;
}

const std::string &NamePrefix::getSite() const {
    return site;
}

void NamePrefix::setSite(const std::string &site) {
    NamePrefix::site = site;
}

const std::string &NamePrefix::getRouter() const {
    return router;
}

void NamePrefix::setRouter(const std::string &router) {
    NamePrefix::router = router;
}

const std::string &NamePrefix::getResource() const {

    return resource;
}

void NamePrefix::setResource(const std::string &resource) {
    NamePrefix::resource = resource;
}


void NamePrefix::show() const {
    printlog("NamePrefix的内容如下");
    printlog("network:" + network);
    printlog("site:" + site);
    printlog("device:" + router);
    printlog("resource:" + resource);
}

bool NamePrefix::operator==(const NamePrefix &np) const {
    bool flag = false;
    if ((this->network == np.network) && (this->site == np.site) &&
        (this->router == np.router) && (this->resource == np.resource)) {
        flag = true;
    }
    return flag;
}

bool NamePrefix::operator<(const NamePrefix& np) const {
    bool flag = false;
    if (network < np.network) {
        flag = true;
    } else if (network > np.network) {
        flag = false;
    } else {
        if (site < np.site) {
            flag = true;
        } else if (site > np.site) {
            flag = false;
        } else {
            if (router < np.router) {
                flag = true;
            } else if (router > np.router) {
                flag = false;
            } else {
                if (resource < np.resource) {
                    flag = true;
                } else {
                    flag = false;
                }
            }
        }
    }
    return flag;
}

NamePrefix::NamePrefix(std::string network, std::string site, std::string router, std::string resource) {
    this->network=network;
    this->site=site;
    this->router=router;
    this->resource=resource;
}

NamePrefix::NamePrefix() {
    this->network="BUPT";
    this->site="417";
    this->resource="zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
}
