/** @file libuv_tcp_client.h
 * @brief
 * @author teng.qing
 * @date 2021/7/20
 */

#include "tcp_client.h"

#include <unistd.h>

#include <atomic>
#include <memory>
#include <thread>
#include <unordered_map>

#include "cim/base/event_loop.h"
#include "cim/base/log.h"
#include "uv.h"

using namespace std;

namespace cim {
namespace base {

#ifdef USE_UV_SEND
struct write_req_t {
    uv_write_t req;
    uv_buf_t buf;
};
#endif

const int kInvalidSocket = -1;
static std::unordered_map<int, TcpClientPtr> g_conn_map{};

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char *)malloc(suggested_size);
    buf->len = suggested_size;
}

TcpClient::TcpClient()
    : tcp_handle_(nullptr)
    , sock_fd_(0)
    , connect_status_(ConnectionStatus::kDefault)
    , connection_cb_(nullptr)
    , message_cb_(nullptr)
    , server_port_(0) {}

TcpClient::~TcpClient() {
    //    cout << " deconstruct " << endl;

    close();
}

bool TcpClient::connect(const std::string &serverIp, uint16_t port) {
    close();

    server_ip_ = serverIp;
    server_port_ = port;

    assert(EventLoop::loop_is_run_);

    tcp_handle_ = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
    int ret = uv_tcp_init(uv_default_loop(), tcp_handle_);
    if (ret == kInvalidSocket) {
        LogWarn("uv_tcp_init error:{}", errno);
        return false;
    }

    struct sockaddr_in remote_addr {};
    ret = uv_ip4_addr(serverIp.c_str(), port, &remote_addr);
    if (ret == kInvalidSocket) {
        LogWarn("uv_ip4_addr error:{}", errno);
        return false;
    }

    connect_status_ = ConnectionStatus::kConnecting;

    auto connect = (uv_connect_t *)malloc(sizeof(uv_connect_t));
    ret = uv_tcp_connect(connect, tcp_handle_, (const struct sockaddr *)&remote_addr, onConnect);
    if (0 == ret) {
        // get socket fd
        uv_fileno((uv_handle_t *)tcp_handle_, &sock_fd_);
        g_conn_map[sock_fd_] = shared_from_this();

        LogDebug("connect to {}:{}, fd={}, handle type={}", serverIp, port, sock_fd_, tcp_handle_->type);
        return true;
    } else {
        LogWarn("connect error:{}", ret);
        close();
    }
    return false;
}

void TcpClient::close() {
    if (tcp_handle_ != nullptr) {
        uv_close((uv_handle_t *)tcp_handle_, nullptr);
        // libuv will free it?
        // free(tcp_handle_);
        tcp_handle_ = nullptr;
    }
    if (sock_fd_ != kInvalidSocket) {
        g_conn_map.erase(sock_fd_);
    }
    connect_status_ = ConnectionStatus::kDisConnected;
}

int TcpClient::send(const char *buff, int len) {
    assert(connect_status_ == ConnectionStatus::kConnectOk);

#ifdef USE_UV_SEND
    auto *req = (write_req_t *)malloc(sizeof(write_req_t));

    char *tempBuf = static_cast<char *>(malloc(len));
    ::memcpy(tempBuf, buff, len);
    req->buf = uv_buf_init(tempBuf, len);

    uv_write((uv_write_t *)req, (uv_stream_t *)tcp_handle_, &req->buf, 1, onWrite);
    return len;
#else
    return ::send(sock_fd_, buff, len, 0);
#endif
}

void TcpClient::setConnectionCallback(const ConnectionCallback &cb) { connection_cb_ = cb; }

void TcpClient::setMessageCallback(const MessageCallback &cb) { message_cb_ = cb; }

void TcpClient::setConnectionStatus(ConnectionStatus status) {
    connect_status_ = status;
    if (connection_cb_) {
        connection_cb_(shared_from_this(), connect_status_);
    }
}

void TcpClient::onTimer() {}

void TcpClient::onConnect(uv_connect_s *req, int status) {
    if (status == 0) {
        auto tcpClient = findTcpClient(req->handle);
        assert(tcpClient != nullptr);
        if (tcpClient != nullptr) {
            tcpClient->setConnectionStatus(ConnectionStatus::kConnectOk);

            LogInfo("connect remote:{} success", tcpClient->remoteAddr());
        }

        // 注册read事件
        uv_read_start(req->handle, alloc_buffer, onRead);
    } else {
        LogDebug("connection lost, changed to {}", status);
        auto tcpClient = findTcpClient(req->handle);
        if (tcpClient != nullptr) {
            tcpClient->close();
            tcpClient->setConnectionStatus(ConnectionStatus::kDisConnected);
        }
    }
}

#ifdef USE_UV_SEND
void TcpClient::onWrite(uv_write_s *req, int status) {
    if (status == 0) {
        // cout << "write succeed!" << endl;
    } else {
        cout << "write error:" << status << endl;
        // uv_close((uv_handle_t*)req,NULL);
    }

    auto *wr = (write_req_t *)req;
    free(wr->buf.base);
    free(wr);
}
#endif

void TcpClient::onRead(uv_stream_s *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        // cout << "recv len=" << nread << ",bff_size=" << buf->len << endl;

        auto tcpClient = findTcpClient(client);
        if (tcpClient) {
            if (tcpClient->message_cb_) {
                tcpClient->message_cb_(tcpClient, buf->base, nread);
            }
        }
    } else {
        if (nread < 0) {
            if (nread != UV_EOF) {
                fprintf(stderr, "Read error %s\n", uv_err_name(nread));
            }
            auto tcpClient = findTcpClient(client);
            if (tcpClient) {
                tcpClient->close();
                tcpClient->setConnectionStatus(ConnectionStatus::kDisConnected);
            }
        }
        free(buf->base);
    }
}

TcpClientPtr TcpClient::findTcpClient(uv_stream_s *handle) {
    int sockFd = kInvalidSocket;
    assert(uv_fileno((uv_handle_t *)handle, &sockFd) == 0);

    if (g_conn_map.find(sockFd) != g_conn_map.end()) {
        return g_conn_map[sockFd];
    } else {
        LogWarn("not find fd={}", sockFd);
    }
    return nullptr;
}

std::string TcpClient::remoteAddr() const { return server_ip_ + ":" + std::to_string(server_port_); }

bool TcpClient::ping(const string &serverIp, uint16_t port) {
#ifdef H_OS_WINDOWS
    return false;
#else
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        LogWarn("socket error:{}", errno);
        return false;
    }

    struct sockaddr_in remote {};
    remote.sin_family = AF_INET;
    remote.sin_port = htons(port);
    remote.sin_addr.s_addr = inet_addr(serverIp.c_str());

    int ret = ::connect(fd, (sockaddr *)&remote, sizeof(remote));
    LogDebug("connect remote {}:{}, ret={}", serverIp, port, ret);

    ::close(fd);
    return ret != -1;
#endif
}

} // namespace base
} // namespace cim