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
    int addrLen=sizeof(addr);
    while(true){
        int cfd= accept(this->serverFd, reinterpret_cast<sockaddr*>(&addr), reinterpret_cast<socklen_t*>(&addrLen));
        pool.commit([this,cfd](){
            sendAndRecv(cfd);
        });
    }
}

bool server::safeRecv(int fd, char *buf, uint32_t n, int flag) {
    uint32_t count = 0;
    uint32_t i = 0;
    while ((i = recv(fd, buf + count, n - count, flag)) > 0) {
        count += i;
        if (count >= n) {
            return true;
        }
    }
    return false;
}

bool server::safeSend(int fd, char *buf, uint32_t n, int flag) {
    uint32_t count = 0;
    uint32_t i = 0;
    while ((i = send(fd, buf + count, n - count, flag)) > 0) {
        count += i;
        if (count >= n) {
            return true;
        }
    }
    return false;
}

void server::sendAndRecv(int cfd) {
    while(true){
        if (!safeRecv(cfd, reinterpret_cast<char *>(&recvSize), sizeof(uint32_t), 0)) {
            ERROR("recv Size")
            close(cfd);
            exit(-1);
        }
        recvBuf.resize(recvSize);
        if (!safeRecv(cfd, const_cast<char *>(recvBuf.data()), recvSize, 0)) {
            ERROR("recv recvBuf")
            close(cfd);
            exit(-1);
        }
        std::cout<<"this thread is: "<<std::this_thread::get_id()<<std::endl;
        std::cout << "recv:" << recvBuf << std::endl;
        sendBuf = "server to client:chgchgchg";
        sendSize = sendBuf.size();
        if (!safeSend(cfd, reinterpret_cast<char *>(&sendSize), sizeof(sendSize), 0)) {
            ERROR("send error")
            exit(-1);
        }
        if (!safeSend(cfd, const_cast<char *>(sendBuf.data()), sendSize, 0)) {
            ERROR("send error")
            exit(-1);
        }
    }
}






