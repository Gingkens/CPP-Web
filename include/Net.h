#pragma once

#include "Request.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <iostream>

#define MAX_SIZE 1024           //缓冲区大小
#define MAX_LINE 1024           //行缓冲区大小
#define RIO_BUFSIZE 8192        //预读缓冲区大小
#define MAX_CONNECT 20          //最大套接字连接个数

using namespace std;


int open_clientfd(const char*hostname, const char*port);
int open_listentfd(const char*port);

class RIO
{
public:
    RIO(int fd);
    string rio_RecvLineb(int flags = 0);
    ssize_t rio_recvLineb(void *usrbuf, size_t maxlen, int flags = 0);
    ssize_t rio_recvNb(void *usrbuf, size_t n, int flags = 0);
    ssize_t rio_send(const void *buf, size_t n, int flags = 0);
    ssize_t rio_recv(void *usrbuf, size_t n, int flags = 0);
    Request read_request();
    vector<string> read_requesthdrs();
    string read_requestCnts(size_t length);
    int get_fd(){return rio_fd;}
private:
    int rio_fd;                 //文件描述符
    int rio_cnt;                //未读的字节数
    char *rio_bufptr;           //指向下一个未读字节的指针
    char rio_buf[RIO_BUFSIZE];  //内部缓冲区
    char buf[MAX_LINE];         //行读取缓冲区
};