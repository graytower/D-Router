//
// Created by suggie on 2022/2/19.
//

#ifndef NDN_NEW_NAMEPREFIX_H
#define NDN_NEW_NAMEPREFIX_H

#include <string>

class NamePrefix {
private:
    std::string network;
    std::string site;
    std::string router;
    std::string resource;

public:
    NamePrefix();

    NamePrefix( std::string network,std::string site,std::string router,std::string resource);

    bool operator<(const NamePrefix& np) const;

    void show() const;

    bool operator==(const NamePrefix &np) const;

    const std::string &getNetwork() const;

    void setNetwork(const std::string &network);

    const std::string &getSite() const;

    void setSite(const std::string &site);

    const std::string &getRouter() const;

    void setRouter(const std::string &router);

    const std::string &getResource() const;

    void setResource(const std::string &resource);
};


#endif //NDN_NEW_NAMEPREFIX_H
