/** @file server.h
 * @brief
 * @author teng.qing
 * @date 2021/7/20
 */
#include "uv_tcp_server.h"

int main() {
    // start server
    UvTcpServer::getInstance()->run(9900);
    // stop server
    //cim::TcpServer::getInstance()->stop();
    return 0;
}