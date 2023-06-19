//
// Created by suggie on 2022/3/16.
//
#include<cstring>
#include <iostream>
#include <fstream>
#include "ConfigFileInStream.h"
#include "Helper.h"

pthread_mutex_t fileMutex;

std::vector<NamePrefix> ConfigFileInStream::readInNamePrefixs(const std::string& configFileName) {
    pthread_mutex_lock(&fileMutex);
    //读入文件中的名称前缀
    std::vector<NamePrefix> nps;
    std::ifstream fin;
    fin.open(configFileName, std::ios::in);
    if (!fin.is_open()) {
        Error("cannot open the file" +configFileName) ;
    }
    while (!fin.eof()) {
        std::string str;
        fin >> str;
        if (str.empty()) {
            continue;
        }
        std::vector<std::string> strVec = split(str, "/");//注意！可能有转义问题！！！
        if (strVec.size() != 4) {
            Error("Data/request.txt 必须 / 分割 4 部分");
        }

        NamePrefix np;
        np.setNetwork(strVec[0]);
        np.setSite(strVec[1]);
        np.setRouter(strVec[2]);
        np.setResource(strVec[3]);
        nps.push_back(np);
    }
    pthread_mutex_unlock(&fileMutex);
    return nps;
}

std::vector<std::string> ConfigFileInStream::split(const std::string &str, const std::string &delim) {
    std::vector<std::string> res;
    if (str.empty()) return res;
    //先将要切割的字符串从string类型转换为char*类型
    char *strs = new char[str.length() + 1]; //不要忘了
    Strcpy_s(strs, str.length() + 1, str.c_str());

    char *d = new char[delim.length() + 1];
    Strcpy_s(d, delim.length() + 1, delim.c_str());

    char *p = strtok(strs, d);
    while (p) {
        std::string s = p; //分割得到的字符串转换为string类型
        res.push_back(s); //存入结果数组
        p = strtok(nullptr, d);
    }
    delete [] strs;
    delete [] d;
    return res;
}

ConfigFileInStream::ConfigFileInStream() {
    pthread_mutex_init(&fileMutex, nullptr);
}

ConfigFileInStream::~ConfigFileInStream() {
    pthread_mutex_destroy(&fileMutex);
}
