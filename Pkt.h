//
// Created by csy on 2023/3/29.
//

#ifndef NDN_NEW_PKT_H
#define NDN_NEW_PKT_H

#include "NamePrefix.h"

enum PktType{
    INVALID = 0, INST=1, DATA, SYNC_INST, SYNC_DATA
};

class Pkt {
protected:
    int id;//包id
    PktType type;//包的类型
    NamePrefix name;//包的名字
    int signature;//表明包产生的源头设备ID

public:
    virtual  ~Pkt()= default;
    int getSignature() const;
    void setSignature(int signature);

    int getId() const;
    void setId(int id);

    PktType getType() const;
    void setType(PktType type);

    void setName(const NamePrefix &name);
    void setName(const std::string& network,const std::string& site,const std::string& routerOrServer, const std::string& resource);

    const NamePrefix &getName() const;
};


#endif //NDN_NEW_PKT_H
