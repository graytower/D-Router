//
// Created by suggie on 2022/4/27.
//

#include <iostream>
#include <string>
#include <fstream>
#include "Helper.h"


pthread_mutex_t lock;
// 带锁的日志输出
void printlog(std::string s)
{
    static bool f = true;
    if (f)
    {
        f = false;
        pthread_mutex_init(&lock, NULL);
    }
    pthread_mutex_lock(&lock);
    std::cout << s << std::endl;
    pthread_mutex_unlock(&lock);
}


// 抛出错误
void Error(std::string s)
{
    Error(s, 1);
}
void Error(string s, int errcode)
{
    printlog("遇到严重错误：" + s);
    exit(errcode);
}


// 安全的字符串复制函数
void Strcpy_s(char *dst, int dstsize, const char *src)
{
    int available = dstsize;
    while((*dst++ = *src++) != 0 && --available > 0);

    if (available == 0)
        Error("字符串拷贝时溢出", 5);
}


string TimevalToString(timeval tv){
    char timeStr[40]={0};
    struct tm* ptm = localtime (&tv.tv_sec);
    strftime (timeStr, sizeof (timeStr), "%Y-%m-%d-%H:%M:%S", ptm);
    long milliseconds = tv.tv_usec / 1000;
    /* 以秒为单位打印格式化后的时间日期，小数点后为毫秒。*/
//    printf ("%s.%03ld\t", timeStr, milliseconds);
    string s(timeStr);
    return s+'.'+to_string(milliseconds);
}

double calTimeDif(timeval minuend, timeval subtrahend){
    return (minuend.tv_sec-subtrahend.tv_sec)*1000+(minuend.tv_usec-subtrahend.tv_usec)/1000;
}

long compareTimeval(const timeval &lhs, const timeval &rhs) {
    if (lhs.tv_sec < rhs.tv_sec)
        return -1;
    if (lhs.tv_sec > rhs.tv_sec)
        return 1;
    return lhs.tv_usec - rhs.tv_usec;
}

