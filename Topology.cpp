//
// Created by suggie on 2021/11/4.
//
#include <fstream>
#include <iostream>
#include <cstring>
#include "Topology.h"

using namespace std;



// 构造函数（私有）
Topology::Topology() {
    this->n = 0;
    this->m_hostNum = 0;
    this->m_routerNum = 0;
    this->m_serverNum = 0;
    data = nullptr;

	//初始化网络拓扑
	init("Data/topo.txt");
};


// 析构函数
Topology::~Topology() {
    delete [] data;
}


// 初始化
int Topology::init(string fileName) {
	ifstream fin;
	fin.open(fileName, ios::in);
	if (!fin.is_open()) {
		std::cerr << "cannot open the file" << fileName << endl;
		return -1;
	}

	fin >> this->n;
	fin >> this->m_routerNum;
	fin >> this->m_serverNum;
	fin >> this->m_hostNum;

	if (n > 0) {
		delete[]data;
		data = new int[n * n];
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				int value;
				fin >> value;
				setEdge(i, j, value);
			}
		}
	}
	return 0;
}

int Topology::getEdge(int i, int j) const {
    if ((i < 0) || (j < 0) || (i >= n) || (j >= n)) {
        return -1;//溢出
    } else {
        return data[i * n + j];
    }
}

bool Topology::setEdge(int i, int j, int value) {
    if ((i < 0) || (j < 0) || (i >= n) || (j >= n)) {
        return false;//溢出
    } else {
        data[i * n + j] = value;
        return true;
    }
}




void Topology::show() const {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << this->getEdge(i, j) << " ";
        }
        cout << endl;
    }
}

Topology &Topology::getInstance() {
    static Topology instance;
    return instance;
}
