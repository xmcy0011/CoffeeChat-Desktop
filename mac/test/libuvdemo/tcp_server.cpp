/** @file tcp_server.h
 * @brief
 * @author teng.qing
 * @date 2021/7/20
 */

#include "tcp_server.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <thread>

using namespace std;

namespace cim {

const int kSocketError = -1;

TcpServer* TcpServer::getInstance() {
    static TcpServer instance;
    return &instance;
}
void TcpServer::run(int listen_port) {
    listen_fd_ = ::socket(PF_INET, SOCK_STREAM, 0);
    if (listen_fd_ == -1) {
        cout << "create socket error:" << errno << endl;
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(listen_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int ret = ::bind(listen_fd_, (sockaddr*)&addr, sizeof(sockaddr_in));
    if (ret == -1) {
        std::cout << "bind socket error:" << errno << std::endl;
        return;
    }

    std::cout << "server listen on :" << listen_port << ",fd=" << listen_fd_ << std::endl;
    // 标识文件描述符为被动socket
    ret = ::listen(listen_fd_, SOMAXCONN);
    if (ret == kSocketError) {
        std::cout << "listen error:" << errno << std::endl;
        return;
    }

    threadProc(); // run in main thread
    //    auto cb = std::bind(&TcpServer::threadProc, this);
    //    std::thread thread(cb);
    //    thread.detach();
}
void TcpServer::stop() {
    run_ = false;
    close(listen_fd_);
}
void TcpServer::threadProc() {
    // 死循环，永不退出
    while (run_) {
        struct sockaddr_in peerAddr {};
        socklen_t sockLen = sizeof(sockaddr_in);
        // will sleep, until one connection coming
        int fd = ::accept(listen_fd_, (sockaddr*)&peerAddr, &sockLen);
        if (fd == kSocketError) {
            return;
        }
        std::cout << "new connect coming,accept..." << std::endl;
        while (true) {
            char buffer[1024] = {};
            // 没有数据时会阻塞
            ssize_t len = recv(fd, buffer, sizeof(buffer), 0); // wait
            if (len == kSocketError) {
                std::cout << "recv error:" << errno << std::endl;
                break;

            } else if (len == 0) {
                std::cout << "recv error:" << errno << std::endl;
                break;

            } else {
                std::cout << "server: " << buffer << std::endl;
                // echo
                len = send(fd, buffer, len, 0);
                if (len == kSocketError) {
                    std::cout << "send error:" << errno << std::endl;
                    break;
                }
            }
        }
        ::close(fd);
        std::cout << "remote " << ::inet_ntoa(peerAddr.sin_addr) << " close the connection" << std::endl;
    }
}

} // namespace cim