/** @file main.h
 * @brief
 * @author teng.qing
 * @date 2021/7/20
 */

#include <iostream>
#include <thread>

#include "tcp_client.h"

int main() {
    cim::TcpClient::runLoopInThread(); // must run before connect

    cim::TcpClientPtr client = std::make_shared<cim::TcpClient>();
    client->setConnectionCallback([](const cim::TcpClientPtr &, cim::ConnectionStatus status) {
        std::cout << "ConnectionCallback: connect status change to " << (int)status << std::endl;
    });
    client->setMessageCallback([](const cim::TcpClientPtr &, char *buf, int len) {
        std::cout << "MessageCallback: " << std::string(buf, len) << std::endl;
    });

    int ret = client->connect("127.0.0.1", 9900);
    if (ret == -1) {
        return 0;
    }

    // mock
    const int kMaxTime = 10;
    for (int i = 1; i <= kMaxTime; ++i) {
        std::string str = "hello" + std::to_string(i);
        // send
        client->send(str.c_str(), str.length());
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "10s later exit..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));

    client->close();

    cim::TcpClient::stopLoop();
    return 0;
}