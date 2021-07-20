/** @file tcp_server.h
 * @brief
 * @author teng.qing
 * @date 2021/7/20
 */

#ifndef MAC_TCP_SERVER_H_
#define MAC_TCP_SERVER_H_

#include <atomic>
#include <string>

namespace cim {

/** @class tcp_server
 * @brief
 */
class TcpServer {
public:
    TcpServer(const TcpServer &) = delete;

    TcpServer(const TcpServer &&) = delete;

    TcpServer operator=(const TcpServer &) = delete;

    static TcpServer *getInstance();

public:
    void run(int listen_port);

    void stop();

private:
    void threadProc();

private:
    TcpServer() : run_(true) {}
    ~TcpServer() = default;

private:
    int listen_fd_;
    std::atomic_bool run_;
};
} // namespace cim

#endif // MAC_TCP_SERVER_H_
