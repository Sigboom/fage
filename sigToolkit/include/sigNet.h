/*************************************************************************
	> File Name: sigNet.h
	> Author: Doni Daniel
	> Mail: sigboom@163.com
	> Created Time: 二 10/22 23:36:22 2019
 ************************************************************************/

#ifndef _SIGNET_H
#define _SIGNET_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <stdarg.h>
#include <thread>

#define MSGLEN 512
#define sendFile(socket, ...) h_sendFile(socket, { __VA_ARGS__ })

using std::string;
using std::initializer_list;

class sigNet {
public:
    int socket_create(int port);
    int socket_connect(int port, string host);
    static int recvMsg(int connfd, string &buffer, int msgLen = MSGLEN, int args = 0);
    static int sendMsg(int connfd, string &buffer, int msglen = 0, int args = 0);
    int h_sendFile(int socketfd, initializer_list<string> filePath);
    int recvFile(int filePort, string host, string filePath);
private:
    static void th_sendFile(int connfd, initializer_list<string> filePath);
    static void th_recvFile(int connfd, string filePath);
};

#endif