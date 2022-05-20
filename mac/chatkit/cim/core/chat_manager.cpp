#include "chat_manager.h"

#include <ctime>

#include "cim/base/util/string_util.h"
#include "cim/core/client.h"
#include "cim/pb/CIM.Message.pb.h"

namespace cim {
namespace core {

const int kTimeSpanHalfHour = 60 * 60;
const int kTimeSpanMin = 10 * 60;

ChatManager& ChatManager::getInstance() {
    static ChatManager instance;
    return instance;
}

std::wstring ChatManager::FormatMsgTime(const int64_t& msgTimeStamp) {
    time_t curTime = time(nullptr);
    tm curDay = *localtime(&curTime);

    time_t msgTime = (time_t)msgTimeStamp;
    tm msgDay = *localtime(&msgTime);

    auto timeSpan = ::abs(msgTimeStamp - (int64_t)curTime);

    if (timeSpan <= kTimeSpanMin) { // 10分钟内，显示刚刚
        return L"刚刚";

    } else if (timeSpan <= kTimeSpanHalfHour) { // 1小时内，显示xx分钟前
        return cim::base::UTF8ToUTF16(std::to_string(timeSpan / 60)) + L"分钟前";

    } else if (curDay.tm_year == msgDay.tm_year) {
        if (curDay.tm_mday == msgDay.tm_mday) { // 今日内，显示时分
            char buffer[20] = {0};
            strftime(buffer, sizeof(buffer), "%H:%M", &msgDay);

            return cim::base::UTF8ToUTF16(buffer);

        } else if (curDay.tm_mday / 7 == msgDay.tm_mday / 7) { // 同一周？显示星期几
            std::wstring weekStr = L"一";

            switch (msgDay.tm_wday) {
                case 0:
                    weekStr = L"天";
                    break;

                case 2:
                    weekStr = L"二";
                    break;

                case 3:
                    weekStr = L"三";
                    break;

                case 4:
                    weekStr = L"四";
                    break;

                case 5:
                    weekStr = L"五";
                    break;

                case 6:
                    weekStr = L"六";
                    break;
            }

            return L"星期" + weekStr;
        }

        // 同一年？显示月/日
        char buffer[20] = {0};
        strftime(buffer, sizeof(buffer), "%m/%d", &msgDay);
        return cim::base::UTF8ToUTF16(buffer);

    } else if (::abs(msgDay.tm_year - curDay.tm_year) <= 1) { // 一年内，年/月
        char buffer[20] = {0};
        strftime(buffer, sizeof(buffer), "%Y/%m", &msgDay);
        return cim::base::UTF8ToUTF16(buffer);
    }

    return L"很久以前";
}

ChatManager::ChatManager() {}

ChatManager::~ChatManager() {}

void ChatManager::sendMessage(uint64_t to, int msg, std::string msg_data) {
    CIM::Message::CIMMsgData data{};
    data.set_from_user_id(Client::getInstance()->GetUserId());
    data.set_from_nick_name("unknow");
    data.set_msg_type((CIM::Def::CIMMsgType)msg);
    data.set_to_session_id(to);
    data.set_msg_data(msg_data.c_str());

    Client::getInstance()->send(CIM::Def::CIMCmdID::kCIM_CID_MSG_DATA, data);
}
void ChatManager::setRecvMsgCallback(const RecvMsgCallback& cb) { recv_cb_ = cb; }

void ChatManager::onHandleData(const IMHeader* header, cim::base::Buffer* buffer) {
    if (header->cmd == kCIM_CID_MSG_DATA) {
        // 解析
        if (recv_cb_ != nullptr) {
            CIM::Message::CIMMsgData data{};
            if (data.ParseFromArray(buffer->data(), buffer->length())) {
                recv_cb_(data);
            }
        }
    }
}

#if 0
        void testFormatMsgTime() {
            // 分钟
            int64_t msgTime = time(nullptr) - rand() % 10 * 60;
            std::wstring minMsg = ChatManager::FormatMsgTime(time(nullptr) - (rand() % 10) * 60);
            LogInfo("{} {}", msgTime, nbase::UTF16ToUTF8(minMsg));

            // 小时
            msgTime = time(nullptr) - (rand() % 8 + 1) * 60 * 60;
            LogInfo("{} {}", msgTime, nbase::UTF16ToUTF8(ChatManager::FormatMsgTime(msgTime)));

            // 星期
            msgTime = time(nullptr) - (rand() % 4 + 1) * 24 * 60 * 60;
            LogInfo("{} {}", msgTime, nbase::UTF16ToUTF8(ChatManager::FormatMsgTime(msgTime)));

            // 月/日
            msgTime = time(nullptr) - ((rand() % 4 + 1) * 24 * 60 * 60 + 7 * 86400);
            LogInfo("{} {}", msgTime, nbase::UTF16ToUTF8(ChatManager::FormatMsgTime(msgTime)));

            // 年/月
            msgTime = time(nullptr) - ((rand() % 100 + 1) * 24 * 60 * 60 + 360 * 86400);
            LogInfo("{} {}", msgTime, nbase::UTF16ToUTF8(ChatManager::FormatMsgTime(msgTime)));

            // 很久以前
            msgTime = time(nullptr) - 2 * 360 * 86400;
            LogInfo("{} {}", msgTime, nbase::UTF16ToUTF8(ChatManager::FormatMsgTime(msgTime)));
        }
#endif

} // namespace core
} // namespace cim
