

#include "NameLSA.h"
#include "Helper.h"

const std::string &NameLSA::getNetwork() const {
    return network;
}

void NameLSA::setNetwork(const std::string &network) {
    NameLSA::network = network;
}

const std::string &NameLSA::getSite() const {
    return site;
}

void NameLSA::setSite(const std::string &site) {
    NameLSA::site = site;
}

const std::string &NameLSA::getRouter() const {
    return router;
}

void NameLSA::setRouter(const std::string &router) {
    NameLSA::router = router;
}

const std::string &NameLSA::getNameLsa() const {
    return nameLSA;
}

int NameLSA::getVersion() const {
    return version;
}

void NameLSA::setVersion(int version) {
    NameLSA::version = version;
}

const std::vector<NamePrefix> &NameLSA::getNamePrefixs() const {
    return namePrefixs;
}

void NameLSA::setNamePrefixs(const std::vector<NamePrefix> &namePrefixs) {
    NameLSA::namePrefixs = namePrefixs;
}


void NameLSA::addNamePrefix(NamePrefix np) {
    this->namePrefixs.push_back(np);
}

void NameLSA::setNameLsa(const std::string &nameLsa) {
    nameLSA = nameLsa;
}

NameLSA::NameLSA() {
    this->network = "BUPT";
    this->site = "417";
    this->version = 0;
    this->nameLSA="NameLSA";
}

void NameLSA::show() const {
    printlog("  NameLSA network:" + this->network);
    printlog("  NameLSA site:" + this->site);
    printlog("  NameLSA device:" + this->router);
    printlog("  NameLSA nameLsa:" + this->nameLSA);
    printlog("  NameLSA version:" + this->version);
    printlog("  NamePrefixNum:" + this->namePrefixs.size());
    for (const auto & namePrefix : namePrefixs) {
        printlog("      NamePrefix network:" + namePrefix.getNetwork());
        printlog("      NamePrefix site:" + namePrefix.getSite());
        printlog("      NamePrefix device:" + namePrefix.getRouter());
        printlog("      NamePrefix resource:" + namePrefix.getResource());
    }
}

void NameLSA::increaseVersion() {
    version++;
}

void NameLSA::deleteNamePrefix(NamePrefix np) {
    for(int i =0;i< this->namePrefixs.size();i++){
        if(namePrefixs[i]==np){
            namePrefixs.erase(namePrefixs.begin()+i);
            break;
        }
    }
}

void NameLSA::deleteNamePrefixByLoc(int loc) {
    if((loc>=0 )&& (loc <namePrefixs.size())){
//        printlog("The nameprefix to be deleted is "+(namePrefixs.begin()+loc)->getRouter()+"/"+(namePrefixs.begin()+loc)->getResource());
        namePrefixs.erase(namePrefixs.begin()+loc);
    } else{
        Error("删除namelsa的名称前缀失败，位置越界");
    }
}
