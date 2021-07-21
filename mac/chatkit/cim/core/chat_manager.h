#ifndef _CHAT_MANAGER_87388056_6B36_4D6E_9B52_29714A89AD17_
#define _CHAT_MANAGER_87388056_6B36_4D6E_9B52_29714A89AD17_

#include <string>

#include "cim/base/log.h"
#include "cim/base/noncopyable.h"
#include "cim/cim_dll.h"

namespace cim {
namespace core {

class CIM_DLL_API ChatManager : public base::noncopyable {
public:
    static ChatManager& getInstance();
    static std::wstring FormatMsgTime(const int64_t& msgTimeStamp);

private:
    ChatManager();
    ~ChatManager();
};

// void CIM_DLL_API testFormatMsgTime();
} // namespace core
} // namespace cim

#endif //_CHAT_MANAGER_87388056_6B36_4D6E_9B52_29714A89AD17_