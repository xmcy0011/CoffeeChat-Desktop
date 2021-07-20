/** @file libuv_tcp_client.h
 * @brief
 * @author teng.qing
 * @date 2021/7/20
 */

#ifndef MAC_LIBUV_TCP_CLIENT_H_
#define MAC_LIBUV_TCP_CLIENT_H_

#include <memory>
#include <string>

struct uv_tcp_s;
struct uv_connect_s;
struct uv_write_s;
struct uv_buf_t;
struct uv_timer_s;
struct uv_stream_s;

namespace cim {

enum class ConnectionStatus {
    kDefault,
    kConnecting,
    kDisConnected,
    kConnected,
};

class TcpClient;

typedef std::shared_ptr<TcpClient> TcpClientPtr;
typedef std::function<void(const TcpClientPtr &, ConnectionStatus status)> ConnectionCallback;
typedef std::function<void(const TcpClientPtr &, char *buf, int len)> MessageCallback;

/** @class libuv_tcp_client
 * @brief
 */
class TcpClient : public std::enable_shared_from_this<TcpClient> {
public:
    TcpClient();

    virtual ~TcpClient();

    TcpClient(const TcpClient &) = delete;

    TcpClient(const TcpClient &&) = delete;

    TcpClient operator=(const TcpClient &) = delete;

    bool connect(const std::string &serverIp, uint16_t port);

    void close();

    void send(const char *buff, int len);

    void setConnectionCallback(const ConnectionCallback &cb);

    void setMessageCallback(const MessageCallback &cb);

public:
    static void runLoopInThread();

    static void stopLoop();

private:
    void setConnectionStatus(ConnectionStatus status);

    void onTimer();

private:
    static void onTimer(uv_timer_s *timer);

    static void onConnect(uv_connect_s *req, int status);

    static void onWrite(uv_write_s *req, int status);

    static void onRead(uv_stream_s *client, ssize_t nread, const uv_buf_t *buf);

    static TcpClientPtr findTcpClient(uv_stream_s *handle);

private:
    uv_tcp_s *tcp_handle_;
    int sock_fd_;

    ConnectionStatus connect_status_;
    ConnectionCallback connection_cb_;
    MessageCallback message_cb_;
};

} // namespace cim
#endif // MAC_LIBUV_TCP_CLIENT_H_
