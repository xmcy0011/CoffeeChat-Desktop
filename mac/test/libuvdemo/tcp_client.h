/** @file libuv_tcp_client.h
  * @brief 
  * @author teng.qing
  * @date 2021/7/20
  */

#ifndef MAC_LIBUV_TCP_CLIENT_H_
#define MAC_LIBUV_TCP_CLIENT_H_

#include <string>

struct uv_tcp_s;
struct uv_connect_s;
struct uv_write_s;
struct uv_timer_s;
//struct uv_loop_s;

enum class ConnectionStatus {
    kDefault,
    kConnecting,
    kDisConnected,
    kConnected,
};

/** @class libuv_tcp_client
  * @brief
  */
class TcpClient {
public:
    TcpClient(const TcpClient &) = delete;

    TcpClient(const TcpClient &&) = delete;

    TcpClient operator=(const TcpClient &) = delete;

    static TcpClient *getInstance();

    bool connect(const std::string &serverIp, uint16_t port);

    void close();

private:
    TcpClient();

    virtual ~TcpClient();

    void setConnectionStatus(ConnectionStatus status);

    static void runLoop();

    static void onTimer(uv_timer_s *timer);

    static void onConnect(uv_connect_s *req, int status);

    static void onWrite(uv_write_s *req, int status);

private:
    uv_tcp_s *tcp_handle_;
    uv_timer_s *timer_;

    ConnectionStatus connect_status_;
};


#endif //MAC_LIBUV_TCP_CLIENT_H_
