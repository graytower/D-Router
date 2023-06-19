//
// Created by suggie on 2022/4/17.
//

#ifndef NDN_NEW_INDEXTABLE_H
#define NDN_NEW_INDEXTABLE_H

#include<vector>
#include"NamePrefix.h"

class IndexTable {
private:
    int version;
    std::vector<NamePrefix> namePrefixs;

public:
    IndexTable();

    int getVersion() const;

    void setVersion(int version);

    const std::vector<NamePrefix> &getNamePrefixs() const;

    void setNamePrefixs(const std::vector<NamePrefix> &namePrefixs);

    bool find(const NamePrefix& np);

    void show();

};


#endif //NDN_NEW_INDEXTABLE_H
