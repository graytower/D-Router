//
// Created by suggie on 2022/3/16.
//

#ifndef NDN_NEW_CONFIGFILEINSTREAM_H
#define NDN_NEW_CONFIGFILEINSTREAM_H

#include <vector>
#include "NamePrefix.h"

class ConfigFileInStream {

public:
    ConfigFileInStream();

    ~ConfigFileInStream();

    static std::vector<NamePrefix> readInNamePrefixs(const std::string& configFileName);

private:
    static std::vector<std::string> split(const std::string &str, const std::string &delim);
};


#endif //NDN_NEW_CONFIGFILEINSTREAM_H
