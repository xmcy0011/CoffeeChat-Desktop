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

    /** @fn
     * @brief 连接，会自动重连。非线程安全
     * @param
     * @return
     */
    void login(const std::string& user_name, const std::string& pwd, const LoginCallback& cb,
               const TimeoutCallback& timeout_cb);

    void setConnectionCallback(const ConnectionCallback& cb);

    void logout();

    ConnectionStatus connStatus();

    uint64_t GetUserId() const;

    int sendRaw(const char* data, const int& len);

    int send(CIM::Def::CIMCmdID cmd_id, const google::protobuf::MessageLite& msg);

private:
    void onConnectionStatusChanged(const TcpClientPtr&, ConnectionStatus status);

    void onMessage(const TcpClientPtr& conn, const char* buf, int len);

    void onHandleData(const IMHeader* header, Buffer* buffer);

    void onTimer();

    uint16_t getSeq();

    void onHandleAuthRsp(const IMHeader* header, Buffer* buffer);

protected:
    Client();
    ~Client() override;

private:
    TcpClientPtr tcp_client_;
    Buffer* recv_buffer_;

    uint64_t last_send_time_;
    uint64_t last_recv_time_;

    LoginCallback login_cb_;
    TimeoutCallback login_timeout_cb_;
    atomic<bool> is_login_;

    std::string user_name_;
    std::string user_pwd_;

    uint64_t user_id_;

    ConnectionCallback connection_cb_;
    atomic<uint16_t> seq_;
};

} // namespace core
} // namespace cim
#endif //_TCPCLIENT_2E029671_CF33_4CE8_9F84_FF10E3867D55_