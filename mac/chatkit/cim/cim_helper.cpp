/** @file cim.cpp
 * @brief
 * @author summer sh
 * @date 2021/2/17
 */

#include "cim/cim.h"

#ifdef _WINDOWS_
#include <WINSOCK2.H>
#endif

namespace cim {

ChatKitConfig g_config{};

int initChatKit(const ChatKitConfig &config) {
#ifdef _WINDOWS_
    // evpp windows下 需要初始化socket
    WSADATA wsaData;

    if (WSAStartup(0x0202, &wsaData) != 0) {
        LogError("windows socket init error");
        return kError;
    }
#endif
    g_config = config;

    // init loop
    cim::base::EventLoop::runInThread();
    cim::core::Client::getInstance();

    // init db
    if (!cim::db::SqliteHelper::getInstance()->init()) {
        return kError;
    }

    // init log setting
    GetInstance().setLevel(config.debugConfig.log_level);

    return kSuccess;
}

CIM_DLL_API void cleanup() {
    // close sqlite
    cim::db::SqliteHelper::getInstance()->cleanup();

    cim::core::Client::getInstance()->logout();
    cim::base::EventLoop::stop();
}

CIM_DLL_API void setChatKitServerInfo(const ServerInfoConfig &info) { g_config.serverInfo = info; }

const ChatKitConfig &getChatKitConfig() { return g_config; }

} // namespace cim