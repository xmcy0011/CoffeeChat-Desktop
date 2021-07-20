/** @file main.h
  * @brief 
  * @author teng.qing
  * @date 2021/7/20
  */

#include "tcp_client.h"
#include <iostream>
#include <thread>

int main() {
    if (!TcpClient::getInstance()->connect("127.0.0.1", 7900)) {
        std::cout << "exit" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}