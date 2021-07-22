#include "cim/core/client.h"

#include <thread>

#include "cim/base/log.h"
#include "cim/cim.h"
#include "cim/cim_def.h"
#include "cim/crypt/md5.h"
#include "cim/pb/CIM.Def.pb.h"
#include "cim/pb/CIM.Login.pb.h"
#include "uv.h"

// std::atomic_uint16_t g_seq = 1;
const uint32_t kMaxBufferLen = 1024 * 10; // 10 KB
const std::string kClientVersion = "0.1"; // 客户端类型

namespace cim {
namespace core {

Client::Client()
    : tcp_client_(nullptr)
    , recv_buffer_(nullptr)
    , timer_(nullptr)
    , login_status_(LoginStatus::kDefault)
    , last_login_time_(0)
    , last_send_time_(0)
    , last_recv_time_(0)
    , user_id_(0)
    , seq_(0) {
    recv_buffer_ = new Buffer();

    // 注册心跳定时器
    auto cb = [](uv_timer_s *handle) { Client::getInstance()->onTimer(handle); };
    timer_ = EventLoop::registerTimer(cb, 1000);
}

Client::~Client() {
    logout();
    if (timer_) {
        EventLoop::removeTimer(timer_);
        timer_ = nullptr;
    }
}

Client *Client::getInstance() {
    static Client client;
    return &client;
}

void Client::login(const std::string &user_name, const std::string &pwd, const LoginCallback &cb,
                   const TimeoutCallback &timeout_cb) {
    logout();

    login_cb_ = cb;
    login_timeout_cb_ = timeout_cb;

    login_status_ = LoginStatus::kLogining;
    last_login_time_ = time(nullptr);

    MD5 md;
    md.update(pwd);

    user_name_ = user_name;
    user_pwd_ = md.toString(); // md5

    if (tcp_client_ == nullptr) {
        tcp_client_ = std::make_shared<TcpClient>();

        // bind callback
        tcp_client_->setConnectionCallback(
            std::bind(&Client::onConnectionStatusChanged, this, std::placeholders::_1, std::placeholders::_2));
        tcp_client_->setMessageCallback(
            std::bind(&Client::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    }

    tcp_client_->connect(cim::getChatKitConfig().serverInfo.ip, cim::getChatKitConfig().serverInfo.gatePort);
    LogInfo("connect server,userName={},pwd={}", user_name, user_pwd_);
}

void Client::setConnectionCallback(const ConnectionCallback &cb) { connection_cb_ = cb; }

void Client::logout() {
    if (login_status_ == LoginStatus::kLoginOk) {
        login_status_ = LoginStatus::kDefault;
        CIM::Login::CIMLogoutReq req;
        req.set_user_id(user_id_);
        req.set_client_type(CIM::Def::kCIM_CLIENT_TYPE_PC_WINDOWS);
        send(CIM::Def::kCIM_CID_LOGIN_AUTH_LOGOUT_REQ, req);
    }
    login_status_ = LoginStatus::kDefault;
    if (tcp_client_) {
        tcp_client_->close();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        tcp_client_ = nullptr;
    }
}

ConnectionStatus Client::connStatus() const { return tcp_client_->connectionStatus(); }

int Client::send(const char *data, const int &len) {
    last_send_time_ = time(nullptr);
    return tcp_client_->send(data, len);
}

int Client::send(CIM::Def::CIMCmdID cmd_id, const google::protobuf::MessageLite &msg) {
    LogDebug("cmd_id={},bodyLen={}", cmd_id, msg.ByteSizeLong());

    IMHeader header{};
    header.cmd = cmd_id;
    header.seq = getSeq();
    header.len = sizeof(IMHeader) + msg.ByteSizeLong();
    header.version = kProtocolVersion;

    Buffer buffer(header.len);
    buffer.AppendInt32((int32_t)header.len);
    buffer.AppendInt16((int16_t)kProtocolVersion);
    buffer.AppendInt16(0);
    buffer.AppendInt16(0);
    buffer.AppendInt16((int16_t)header.cmd);
    buffer.AppendInt16((int16_t)header.seq);
    buffer.AppendInt16(0);

    // buffer.Write(&header, sizeof(IMHeader));
    msg.SerializeToArray(buffer.WriteBegin(), msg.ByteSizeLong());
    buffer.WriteBytes(msg.ByteSizeLong());

    if (connStatus() == ConnectionStatus::kConnectOk) {
        return send(buffer.data(), buffer.length());
    } else {
        LogWarn("connection disconnect");
    }

    return kError;
}

void Client::onConnectionStatusChanged(const TcpClientPtr &conn, ConnectionStatus status) {
    LogInfo("connection status={}", status);

    // 登录请求
    if (status == ConnectionStatus::kConnectOk) {
        CIM::Login::CIMAuthReq req;
        req.set_user_name(user_name_);
        req.set_user_pwd(user_pwd_);
        req.set_client_version(kClientVersion);
        req.set_client_type(CIM::Def::kCIM_CLIENT_TYPE_PC_WINDOWS);
        send(CIM::Def::kCIM_CID_LOGIN_AUTH_REQ, req);
        LogInfo("connect success, login ... userName={},pwd={}", user_name_, user_pwd_);
    } else {
        if (login_status_ == LoginStatus::kLogining) {
            if (login_cb_) {
                CIM::Login::CIMAuthRsp rsp;
                rsp.set_result_code(CIM::Def::CIMErrorCode::kCIM_ERR_INTERNAL_ERROR);
                rsp.set_result_string("连接服务器失败，网络异常或者服务器异常");
                login_cb_(rsp);
            }
        }
        logout();
    }

    if (connection_cb_) {
        connection_cb_(conn, status);
    }
}

void Client::onMessage(const TcpClientPtr &conn, const char *buf, int len) {
    LogDebug("remoteIp={},dataLen={}", conn->remoteAddr(), len);

    if (static_cast<unsigned long>(len) <= sizeof(IMHeader)) {
        LogWarn("bad packet");
        return;
    }

    recv_buffer_->Write(buf, len);

    // tcp 粘包
    while (true) {
        IMHeader header{};
        header.len = static_cast<uint32_t>(recv_buffer_->ReadInt32());          // 头+数据部的总长
        int reset_len = recv_buffer_->length() - header.len + sizeof(uint32_t); // 注意已读4字节

        // 坏包，数据部长度超过buffer里面的，全部丢弃。
        if (header.len - sizeof(uint32_t) > recv_buffer_->size()) {
            break;
        }

        if (header.len < kMaxBufferLen) {
            header.version = static_cast<uint16_t>(recv_buffer_->ReadInt16());
            header.flag = static_cast<uint16_t>(recv_buffer_->ReadInt16());
            header.service_id = static_cast<uint16_t>(recv_buffer_->ReadInt16());
            header.cmd = static_cast<uint16_t>(recv_buffer_->ReadInt16());
            header.seq = static_cast<uint16_t>(recv_buffer_->ReadInt16());
            header.reserved = static_cast<uint16_t>(recv_buffer_->ReadInt16());

            onHandleData(&header, recv_buffer_);

            assert(reset_len == recv_buffer_->length());
            // buffer->Skip(header->len);

            if (recv_buffer_->length() <= 0) {
                break;

            } else if (recv_buffer_->length() < sizeof(IMHeader)) {
                LogWarn("packet not full");
                break;
            }

        } else {
            LogWarn("to large packet,len={}", recv_buffer_->size());
            break;
        }
    }

    recv_buffer_->Reset();
}

void Client::onHandleData(const IMHeader *header, Buffer *buffer) {
    LogDebug("cmd={},seq={},len={}", header->cmd, header->seq, header->len);

    switch (header->cmd) {
        case CIM::Def::kCIM_CID_LOGIN_AUTH_RSP:
            onHandleAuthRsp(header, buffer);
            break;

        default:
            LogInfo("unknown cmd={}", header->cmd);
            buffer->Skip(header->len);
            break;
    }
}

void Client::onTimer(uv_timer_s *handle) {
    // handle->start_id
    uint64_t curTime = time(nullptr);

    // 登录超时检测
    if (loginStatus() == LoginStatus::kLogining) {
        uint16_t timeout = cim::getChatKitConfig().loginConfig.login_time_out;
        if ((curTime - last_login_time_) > timeout) {
            if (login_timeout_cb_) {
                login_timeout_cb_();
            }
            LogInfo("login timeout");
        }
    }

    // 心跳保活
    if (loginStatus() == LoginStatus::kLoginOk) {
        uint16_t timeout = cim::getChatKitConfig().loginConfig.keep_alive_time_out;
        if ((curTime - last_recv_time_) > timeout) {
            logout();
            if (connection_cb_) {
                connection_cb_(tcp_client_, ConnectionStatus::kDisConnected);
            }
            LogInfo("heartbeat timeout");
        }
    }
}

uint16_t Client::getSeq() {
    ++seq_;
    return seq_;
}

void Client::onHandleAuthRsp(const IMHeader *header, Buffer *buffer) {
    CIM::Login::CIMAuthRsp rsp;

    PARSE_PB_AND_CHECK(rsp, buffer);

    LogInfo("result_code={},result_string={}", rsp.result_code(), rsp.result_string());

    // 登录失败，断开tcp连接
    if (rsp.result_code() != CIM::Def::CIMErrorCode::kCIM_ERR_SUCCESS) {
        logout();
    }

    if (login_cb_) {
        login_cb_(rsp);
    }
}

} // namespace core
} // namespace cim