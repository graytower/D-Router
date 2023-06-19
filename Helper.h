//
// Created by suggie on 2022/4/27.
//

#ifndef NDN_NEW_HELPER_H
#define NDN_NEW_HELPER_H
#include "Interest.h"
#include "Data.h"


using namespace std;


void Error(string s);
void Error(string s, int errcode);
void Strcpy_s(char *dst, int dstsize, const char *src);
void printlog(string s);

string TimevalToString(timeval tv);
double calTimeDif(timeval minuend, timeval subtrahend);
long compareTimeval(const timeval &lhs, const timeval &rhs);
#endif //NDN_NEW_HELPER_H
