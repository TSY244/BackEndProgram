//
// Created by 12414 on 2023/3/1.
//

#ifndef SIMPLEBACKENDPROGRAM_SERVER_HPP
#define SIMPLEBACKENDPROGRAM_SERVER_HPP
#include "unistd.h"
#include "arpa/inet.h"
#include<iostream>
#include<sys/epoll.h>
#include "../threadPoll/threadpool.h"
#define PORT 8888
#define ERROR(error) std::cout<<"[ "<<__LINE__<<" ]  "<<error<<" failed "<<std::endl;
class server {
private:
    int serverFd;
    uint32_t recvSize;
    uint32_t sendSize;
    std::string recvBuf;
    std::string sendBuf;
    sockaddr_in addr{};
public:
    server();
    inline ~server(){
        close(serverFd);
    }
private:
    bool safeRecv(int fd,char* buf,uint32_t n,int flag);
    bool safeSend(int fd,char* buf,uint32_t n,int flag);
    void sendAndRecv(int cfd);
    std::threadpool pool{50};
};


#endif //SIMPLEBACKENDPROGRAM_SERVER_HPP
