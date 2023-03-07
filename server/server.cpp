//
// Created by 12414 on 2023/3/1.
//

#include "server.hpp"

server::server() {
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {

        ERROR("socket")
        exit(-1);
    }
    int option = 1;
    if ((setsockopt(serverFd, SOL_SOCKET, SO_REUSEPORT, (const char *) &option, sizeof(option))) == -1) {
        ERROR("setsockopt")
        close(serverFd);
        exit(-1);
    }

    addr.sin_port = htons(8888);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htons(INADDR_ANY);

    if ((bind(serverFd, (sockaddr *) &addr, sizeof(addr))) == -1) {
        ERROR("bind")
        close(serverFd);
        exit(-1);
    }

    if ((listen(serverFd, 100)) == -1) {
        std::cout << "listen error" << std::endl;
        close(serverFd);
        exit(-1);
    }
    epfd = epoll_create(10);
    if (epfd == -1) {
        ERROR(epoll_create)
        exit(-1);
    }

    ev.data.fd = serverFd;
    ev.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serverFd, &ev);

    sendAndRecv();

}

bool server::safeRecv(int fd, char *buf, uint32_t n, int flag) {
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

bool server::safeSend(int fd, char *buf, uint32_t n, int flag){
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

void server::sendAndRecv() {
    epoll_event evs[1024]{0};
    evsSize = sizeof(evs) / sizeof(evs[0]);
    while (1) {
        epollNum = epoll_wait(epfd, evs, evsSize, -1);
        std::cout << " epollNum = " << epollNum << std::endl;
        for (int i = 0; i < epollNum; ++i) {
            int temFd = evs[i].data.fd;
            if (temFd == serverFd) {
                //不需要保存信息
                int cFd= accept(temFd,NULL,NULL);
                if(cFd==-1){
                    ERROR("accept")
                    exit(-1);
                }
                std::cout<<"succeed"<<std::endl;
                ev.events=EPOLLIN;
                ev.data.fd=temFd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,cFd,&ev);
            } else{
                if(!safeRecv(temFd,reinterpret_cast<char*>(&recvSize),sizeof (uint32_t),0)){
                    ERROR("recv Size")
                    epoll_ctl(epfd,EPOLL_CTL_DEL,temFd,NULL);
                    close(temFd);
                    exit(-1);
                }
                recvBuf.resize(recvSize);
                if(!safeRecv(temFd,const_cast<char*>(recvBuf.data()),recvSize,0)){
                    ERROR("recv recvBuf")
                    epoll_ctl(epfd,EPOLL_CTL_DEL,temFd,NULL);
                    close(temFd);
                    exit(-1);
                }
                std::cout<<"recv:"<<recvBuf<<std::endl;
                std::cout<<"send:";
                sendBuf="server to client:chgchgchg";
                sendSize=sendBuf.size();
                if(!safeSend(temFd,reinterpret_cast<char*>(&sendSize),sizeof (sendSize),0)){
                    ERROR("send error")
                    exit(-1);
                }
                if(!safeSend(temFd,const_cast<char*>(sendBuf.data()),sendSize,0)){
                    ERROR("send error")
                    exit(-1);
                }
            }
        }
    }
}

