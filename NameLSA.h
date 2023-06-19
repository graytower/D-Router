//
// Created by 22963 on 2022/2/20.
//

#ifndef NDN_NEW_NAMELSA_H
#define NDN_NEW_NAMELSA_H

#include <iostream>
#include <string>
#include <vector>
#include "NamePrefix.h"

class NameLSA {
private:
    std::string network;
    std::string site;
    std::string router;
    std::string nameLSA;
    int version;
    std::vector<NamePrefix> namePrefixs;

public:
    NameLSA();

    void show() const;

    void setNameLsa(const std::string &nameLsa);

    const std::string &getNetwork() const;

    void setNetwork(const std::string &network);

    const std::string &getSite() const;

    void setSite(const std::string &site);

    const std::string &getRouter() const;

    void setRouter(const std::string &router);

    const std::string &getNameLsa() const;

    int getVersion() const;

    void setVersion(int version);

    void increaseVersion();

    const std::vector<NamePrefix> &getNamePrefixs() const;

    void setNamePrefixs(const std::vector<NamePrefix> &namePrefixs);

    void addNamePrefix(NamePrefix np);

    void deleteNamePrefix(NamePrefix np);

    void deleteNamePrefixByLoc(int  loc);

};


#endif //NDN_NEW_NAMELSA_H
