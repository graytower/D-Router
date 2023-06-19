//
// Created by suggie on 2021/11/17.
//

#ifndef NDNROUTER_CSRECORD_H
#define NDNROUTER_CSRECORD_H

#include<string>
#include "NamePrefix.h"
class CSRecord{
private:
    NamePrefix namePrefix;
    std::string content;

public:
    bool operator<(const CSRecord& other) const;

    const std::string &getContent() const;

    void setContent(const std::string &content);

    const NamePrefix &getNamePrefix() const;

    void setNamePrefix(const NamePrefix &namePrefix);


};



#endif //NDNROUTER_CSRECORD_H
