//
// Created by 12414 on 2023/3/5.
//

#include "client.hpp"

client::client() {
    cfd= socket(AF_INET,SOCK_STREAM,0);
    if(cfd==-1){
        ERROR("create client")
        exit(-1);
    }
    sockaddr_in addr;
    addr.sin_port= htons(PORT);
    inet_pton(AF_INET," 172.24.51.188",&addr.sin_addr.s_addr);
    addr.sin_family=AF_INET;

    int sfd= connect(cfd,reinterpret_cast<sockaddr*>(&addr),sizeof (addr));
    if (sfd==-1){
        ERROR("connect")
        exit(-1);
    }
    std::cout<<"connect succeed"<<std::endl;

    while(1){
        //send
        std::cout<<"send:";
        sendBuf="client to serverer: zjyzjyzjy";
        sendSize=sendBuf.size();
        if(!safeSend(cfd,reinterpret_cast<char*>(&sendSize),sizeof (sendSize),0)){
            ERROR("send error")
            exit(-1);
        }
        if(!safeSend(cfd,const_cast<char*>(sendBuf.data()),sendSize,0)){
            ERROR("send error")
            exit(-1);
        }
        //recv
        if(!safeRecv(cfd,reinterpret_cast<char*>(&recvSize),sizeof (uint32_t),0)){
            ERROR("recv Size")
            close(cfd);
        }
        recvBuf.resize(recvSize);
        if(!safeRecv(cfd,const_cast<char*>(recvBuf.data()),recvSize,0)){
            ERROR("recv recvBuf")
            close(cfd);
        }
        std::cout<<"recv:"<<recvBuf<<std::endl;
    }

}

bool client::safeRecv(int fd, char *buf, uint32_t n, int flag) {
    uint32_t count=0;
    uint32_t i=0;
    while((i= recv(fd,buf+count,n-count,flag))>0){
        count+=i;
        if(count>=n){
            return true;
        }
    }
    return false;
}

bool client::safeSend(int fd, char *buf, uint32_t n, int flag){
    uint32_t count=0;
    uint32_t i=0;
    while((i= send(fd,buf+count,n-count,flag))>0){
        count+=i;
        if(count>=n){
            return true;
        }
    }
    return false;
}
