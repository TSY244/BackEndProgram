//
// Created by 12414 on 2023/3/5.
//

#ifndef SIMPLEBACKENDPROGRAM_CLIENT_HPP
#define SIMPLEBACKENDPROGRAM_CLIENT_HPP
#include "unistd.h"
#include "arpa/inet.h"
#include<iostream>

#define PORT 8888
#define ERROR(error) std::cout<<"[ "<<__LINE__<<" ]  "<<error<<" failed "<<std::endl;

class client {
private:
    int cfd;
    uint32_t sendSize;
    uint32_t recvSize;
    std::string sendBuf;
    std::string recvBuf;
public:
    client();
    inline ~client(){
        close(cfd);
    }
private:
    void sendAndRecv(int cfd);
    bool safeRecv(int fd,char* buf,uint32_t n,int flag);
    bool safeSend(int fd,char* buf,uint32_t n,int flag);
};


#endif //SIMPLEBACKENDPROGRAM_CLIENT_HPP
