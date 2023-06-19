//
// Created by suggie on 2021/11/17.
//
#include <string>
#include "CSRecord.h"


const NamePrefix &CSRecord::getNamePrefix() const {
    return namePrefix;
}

void CSRecord::setNamePrefix(const NamePrefix &namePrefix) {
    CSRecord::namePrefix = namePrefix;
}

const std::string &CSRecord::getContent() const {
    return content;
}

void CSRecord::setContent(const std::string &content) {
    CSRecord::content = content;
}

bool CSRecord::operator<(const CSRecord &other) const {
    return (this->namePrefix < other.namePrefix);
}

