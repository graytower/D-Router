
#ifndef NDN_NEW_DATA_H
#define NDN_NEW_DATA_H


#include<iostream>
#include "NamePrefix.h"
#include "Pkt.h"


class Data:public Pkt{
private:
    std::string content;

public:
    Data();
    Data(Data const &d);
    Data(int pktid,NamePrefix np,int signature=-6,std::string ct="zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");

    const std::string &getContent() const;
    void setContent(const std::string &content);


    void show();
};


#endif //NDN_NEW_DATA_H
