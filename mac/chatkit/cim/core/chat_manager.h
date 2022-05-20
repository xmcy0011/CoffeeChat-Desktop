#ifndef _CHAT_MANAGER_87388056_6B36_4D6E_9B52_29714A89AD17_
#define _CHAT_MANAGER_87388056_6B36_4D6E_9B52_29714A89AD17_

#include <functional>
#include <string>

#include "cim/base/buffer.h"
#include "cim/base/log.h"
#include "cim/base/noncopyable.h"
#include "cim/cim_dll.h"
#include "cim/core/client.h"
#include "cim/pb/CIM.Message.pb.h"

namespace cim {
namespace core {

typedef std::function<void(CIM::Message::CIMMsgData)> RecvMsgCallback;

class CIM_DLL_API ChatManager : public base::noncopyable {
public:
    friend Client;

    static ChatManager& getInstance();
    static std::wstring FormatMsgTime(const int64_t& msgTimeStamp);

public:
    void sendMessage(uint64_t to, int msg, std::string msg_data);

    void setRecvMsgCallback(const RecvMsgCallback& cb);

protected:
    void onHandleData(const IMHeader* header, cim::base::Buffer* buffer);

private:
    ChatManager();
    ~ChatManager();

private:
    RecvMsgCallback recv_cb_;
};

// void CIM_DLL_API testFormatMsgTime();
} // namespace core
} // namespace cim

#endif //_CHAT_MANAGER_87388056_6B36_4D6E_9B52_29714A89AD17_