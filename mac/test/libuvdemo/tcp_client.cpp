/** @file libuv_tcp_client.h
  * @brief 
  * @author teng.qing
  * @date 2021/7/20
  */

#include "tcp_client.h"
#include "uv.h"

#include <iostream>
#include <thread>

using namespace std;

TcpClient *TcpClient::getInstance() {
    static TcpClient instance;
    return &instance;
}

bool TcpClient::connect(const std::string &serverIp, uint16_t port) {
    close();

    tcp_handle_ = (uv_tcp_t *) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(uv_default_loop(), tcp_handle_);

    struct sockaddr_in remote_addr{};
    uv_ip4_addr(serverIp.c_str(), port, &remote_addr);

    int sockFd;
    uv_fileno((uv_handle_t *) tcp_handle_, &sockFd);

    cout << "connect to " << serverIp << ":" << port << "fd=" << sockFd << endl;
    connect_status_ = ConnectionStatus::kConnecting;

    auto connect = (uv_connect_t *) malloc(sizeof(uv_connect_t));
    if (0 == uv_tcp_connect(connect, tcp_handle_, (const struct sockaddr *) &remote_addr, onConnect)) {
        cout << "connect..." << endl;
        return true;
    } else {
        close();
    }
    return false;
}

TcpClient::TcpClient() :
        tcp_handle_(nullptr),
        timer_(nullptr),
        connect_status_(ConnectionStatus::kDefault) {
    timer_ = (uv_timer_s *) malloc(sizeof(uv_timer_s));
    uv_timer_init(uv_default_loop(), timer_);
    uv_timer_start(timer_, onTimer, 0, 1000);

    std::thread t([&]() {
        TcpClient::runLoop();
    });
    t.detach();
}

void TcpClient::close() {
    if (tcp_handle_ != nullptr) {
        uv_close((uv_handle_t *) tcp_handle_, nullptr);
        free(tcp_handle_);
        tcp_handle_ = nullptr;
    }
    connect_status_ = ConnectionStatus::kDisConnected;
}

TcpClient::~TcpClient() {
    cout << "deconstruct" << endl;

    close();
    uv_timer_stop(timer_);
    free(timer_);

    uv_stop(uv_default_loop());
}

void TcpClient::setConnectionStatus(ConnectionStatus status) {
    connect_status_ = status;
}

void TcpClient::onConnect(uv_connect_s *req, int status) {
    if (status == 0) {
        cout << "connect success" << endl;
        TcpClient::getInstance()->setConnectionStatus(ConnectionStatus::kConnected);
    } else {
        cout << "connection lost, changed to " << status << endl;
        TcpClient::getInstance()->close();
    }
}

void TcpClient::onWrite(uv_write_s *req, int status) {
    if (status == 0) {
        printf("write succeed!");
    } else {
        printf("write error");
        //uv_close((uv_handle_t*)req,NULL);
    }
}

void TcpClient::runLoop() {
    auto loop = uv_default_loop();
    uv_run(loop, UV_RUN_DEFAULT);
    cout << "uv loop is success stop" << endl;
}

void TcpClient::onTimer(uv_timer_s *timer) {
    //std::cout << "timer from libuv, id=" << timer->start_id << ",time=" << time(nullptr) << std::endl;
}
