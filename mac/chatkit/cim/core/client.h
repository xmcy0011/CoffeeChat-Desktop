/** @file TcpClient.h
 * @brief TcpClient
 * @author fei.xu
 * @date 2020/8/26
 */

#ifndef _TCPCLIENT_2E029671_CF33_4CE8_9F84_FF10E3867D55_
#define _TCPCLIENT_2E029671_CF33_4CE8_9F84_FF10E3867D55_

#include <google/protobuf/message_lite.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "cim/base/buffer.h"
#include "cim/base/noncopyable.h"
#include "cim/base/tcp_client.h"
#include "cim/cim_def.h"
#include "cim/core/callback.h"

using namespace std;
using namespace cim::base;

namespace cim {
namespace core {

/**
 * 登录状态
 */
enum class LoginStatus {
    kDefault,
    kLogining,
    kLoginOk,
};

/** @class Client
 * @brief 封装和服务器通信的类，必须认证后才可使用
 * @author fei.xu
 * @date 2021/2/2
 */
class CIM_DLL_API Client : public cim::base::noncopyable {
public:
    /** @fn
     * @brief 获取单例，线程安全
     * @param
     * @return
     */
    static Client* getInstance();

    /**
     * 连接，会自动重连。非线程安全
     * @param user_name: 用户名
     * @param pwd: 明文密码，SDK中会进行加密
     * @param cb: 登录结果
     * @param timeout_cb: 登录超时回调
     */
    void login(const std::string& user_name, const std::string& pwd, const LoginCallback& cb,
               const TimeoutCallback& timeout_cb);

    /**
     * 绑定连接状态改变回调
     * @param cb: 回调
     */
    void setConnectionCallback(const ConnectionCallback& cb);

    /**
     * 退出登录
     */
    void logout();

    /**
     * 获取连接状态
     * @return
     */
    ConnectionStatus connStatus() const;

    /**
     * 获取登录状态
     * @return
     */
    LoginStatus loginStatus() const { return login_status_; }

    /**
     * 获取用户ID
     * @return
     */
    uint64_t GetUserId() const { return user_id_; }

    /**
     * 发送裸数据
     * @param data：缓冲区
     * @param len：数据长度
     * @return >0：成功发送长度，=0：对端关闭连接，<0: 错误
     */
    int send(const char* data, const int& len);

    /**
     * 发送信令
     * @param cmd_id：命令ID
     * @param msg：数据部内容
     * @return
     */
    int send(CIM::Def::CIMCmdID cmd_id, const google::protobuf::MessageLite& msg);

private:
    void onConnectionStatusChanged(const TcpClientPtr&, ConnectionStatus status);

    void onMessage(const TcpClientPtr& conn, const char* buf, int len);

    void onHandleData(const IMHeader* header, Buffer* buffer);

    void onTimer(uv_timer_s* handle);

    uint16_t getSeq();

    void onHandleAuthRsp(const IMHeader* header, Buffer* buffer);

protected:
    Client();
    ~Client() override;

private:
    TcpClientPtr tcp_client_;
    Buffer* recv_buffer_;
    uv_timer_s* timer_;

    LoginStatus login_status_;

    uint64_t last_login_time_;
    uint64_t last_send_time_;
    uint64_t last_recv_time_;

    LoginCallback login_cb_;
    TimeoutCallback login_timeout_cb_;

    std::string user_name_;
    std::string user_pwd_;

    uint64_t user_id_;

    ConnectionCallback connection_cb_;
    atomic<uint16_t> seq_;
};

} // namespace core
} // namespace cim
#endif //_TCPCLIENT_2E029671_CF33_4CE8_9F84_FF10E3867D55_