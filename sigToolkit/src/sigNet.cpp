/*************************************************************************
	> File Name: sigNet.cpp
	> Author: Doni Daniel
	> Mail: sigboom@163.com
	> Created Time: 二 10/22 23:15:01 2019
 ************************************************************************/

#include "../include/sigNet.h"

using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::thread;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::initializer_list;

int sigNet::socket_create(int port) {
    int socketfd;
    struct sockaddr_in sock_addr;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("socket_create");
        return -1;
    }

    sock_addr.sin_family = AF_INET; 
    sock_addr.sin_port = htons(port); 
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY); 

    if (bind(socketfd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1) { 
        perror("bind");
        return -1;
    }   
    if(listen(socketfd, 10) == -1) {
        perror("listen");
        return -1;
    }
    return socketfd;
}

int sigNet::socket_connect(int port, string host) {
    int socketfd;
    struct sockaddr_in dest_addr;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("socket_connect");
        return -1;
    }

    dest_addr.sin_family = AF_INET; 
    dest_addr.sin_port = htons(port); 
    dest_addr.sin_addr.s_addr = inet_addr(host.c_str()); 

    if (connect(socketfd, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == -1) { 
        perror("connect");
        return -1;
    } 
    return socketfd;
}

int sigNet::recvMsg(int connfd, string &buffer, int msgLen, int args) {
    char recvBuffer[msgLen];
    int n = recv(connfd, recvBuffer, msgLen, args);
    buffer.clear();
    buffer = recvBuffer;
    return n;
}

int sigNet::sendMsg(int connfd, string &buffer, int msgLen, int args) {
    if (msgLen == 0) msgLen = buffer.length() + 1;
    char sendBuffer[msgLen];
    strncpy(sendBuffer, buffer.c_str(), msgLen);
    int n = send(connfd, sendBuffer, msgLen, args);
    return n;
}

void sigNet::pd_sendFile(int socketfd, vector<string> filePath) {
    int connfd;
    if ((connfd = accept(socketfd, (struct sockaddr *)NULL, NULL)) < 0) throw -4;
    cout << "file connect successfully!" << endl;

    for (auto ptr = filePath.begin(); ptr != filePath.end(); ++ptr) {
        string msg = "send " + *ptr;
        sendMsg(connfd, msg);
        recvMsg(connfd, msg);
        if (msg == "check") {
            ifstream file(*ptr);
            if (file.is_open()) {
                string temp;
                int n = 0;
                while (!file.eof()) {
                    file >> temp;
                    n = sendMsg(connfd, temp);
                    if (n <= 0) break;
                }
            }
            file.close();
        }
    }
    close(connfd);
    close(socketfd);
    exit(0);
}

int sigNet::sendFile(int socketfd, vector<string> filePath) {
    int pid = fork();
    if (pid) return pid;
    pd_sendFile(socketfd, filePath);
    return 0;
}

int sigNet::sendFile(int socketfd, string filePath) {
    vector<string> filesPath;
    filesPath.push_back(filePath);
    return sendFile(socketfd, filesPath);
}

void sigNet::th_recvFile(int connfd, string filePath) {
    string msg;
    int pos = 0;
    while (recvMsg(connfd, msg) > 0) {
        if (msg == "END") break;
        if ((pos = msg.find("send")) != string::npos) {
            string logPath = filePath + msg.substr(pos + 5);
            ofstream logFile(logPath);
            msg = "check";
            sendMsg(connfd, msg);
            while(recvMsg(connfd, msg) > 0) {
                if (msg == "EOF") break;
                logFile << msg << flush;
            }
            logFile.close();
        }
    }
    close(connfd);
    exit(0);
}

int sigNet::recvFile(int filePort, string host, string filePath) {
    int connfd = socket_connect(filePort, host);
    thread f_thread(th_recvFile, connfd, filePath);
    f_thread.detach();
    return 0;
}
