#include "user_manager.h"

#include "cim/base/log.h"
#include "cim/cim.h"
#include "cim/crypt/md5.h"
#include "nlohmann/json.hpp"
#include "restclient-cpp/restclient.h"

// for convenience
using json = nlohmann::json;

namespace cim {
namespace core {

const double kHttpTimeout = 3; // s
const std::string kUrlRegisterUser = "/user/register";

const std::string kDefaultHttpErrorMsg = "未知错误";

UserManager *UserManager::getInstance() {
    static UserManager instance;
    return &instance;
}

bool UserManager::registerUser(std::string userName, std::string userPwd, std::string nickName, HttpResponseBase &out) {
    std::string url = "http://" + cim::getChatKitConfig().serverInfo.ip + ":" +
                      std::to_string(cim::getChatKitConfig().serverInfo.httpPort) + kUrlRegisterUser;

    MD5 md;
    md.update(userPwd);
    std::string pwd = md.toString();

    json root;
    root["userName"] = userName;
    root["userNickName"] = nickName;
    root["userPwd"] = pwd;

    LogInfo("register url={},userName={},userPwd={},nickName={}", url, userName, pwd, nickName);

    auto res = RestClient::post(url, "application/json; charset=utf-8", root.dump());

    if (res.code != 200) {
        out.code = CIM::Def::CIMErrorCode::kCIM_ERR_INTERNAL_ERROR;
        out.msg = "server unavailable";
        LogWarn("register error, code={},msg={}", out.code, out.msg);
        return false;

    } else {
        json value = json::parse(res.body);

        if (!value.is_null()) {
            out.code = value["error_code"].get<int>();
            out.msg = value["error_msg"].get<std::string>();

            LogInfo("register code={},msg={}", out.code, out.msg);

            if (out.code == CIM::Def::CIMErrorCode::kCIM_ERR_SUCCESS) {
                return true;
            } else {
                return false;
            }
        } else {
            LogInfo("parse json error:{}", res.body);
        }
    }
    return false;
}

UserManager::UserManager() = default;

} // namespace core
} // namespace cim
