//
// Created by 22963 on 2022/2/11.
//
#include<cstring>
#include "Data.h"
#include "Helper.h"

using namespace std;

const std::string &Data::getContent() const {
    return content;
}

void Data::setContent(const std::string &content) {
    Data::content = content;
}


void Data::show(){
    printlog("Data的内容如下");
    printlog("network:" + name.getNetwork());
    printlog("site:" + name.getSite());
    printlog("device:" + name.getRouter());
    printlog("resource:" + name.getResource());
    printlog("content:" + content);
}

Data::Data() {
  type=DATA;
  content="zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
}

Data::Data(int pktid, NamePrefix np, int sn, std::string ct) {
    type=DATA;
    id=pktid;
    name=np;
    signature=sn;
    content=ct;
}

Data::Data(const Data &d) {
    type=DATA;
    id=d.id;
    name=d.name;
    signature=d.signature;
    content=d.content;
}


