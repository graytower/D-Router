//
// Created by suggie on 2022/2/23.
//

#ifndef NDN_NEW_FIB_H
#define NDN_NEW_FIB_H


#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include "NamePrefix.h"

namespace std
{
    template<>
    struct hash<NamePrefix>: public __hash_base<size_t, NamePrefix>
    {
        size_t operator()(const NamePrefix& np) const noexcept
        {
            return (hash<string>()(np.getNetwork())) ^ (hash<string>()(np.getSite()) << 1)+(hash<string>()(np.getRouter()) << 2)+(hash<string>()(np.getResource()));
        }
    };
}

typedef std::unordered_map<NamePrefix,std::pair<int,int>> FIB_MAP;
class FIB {
private:
    FIB_MAP fibMap;

public:
    pthread_mutex_t tableMutex;

    FIB();
    ~FIB();

    void insertRecord(std::pair<NamePrefix,std::pair<int,int>> fibRecord);

    void clear();

    FIB_MAP::iterator match(const NamePrefix& np);

    const FIB_MAP &getFibMap() const;

};


#endif //NDN_NEW_FIB_H
