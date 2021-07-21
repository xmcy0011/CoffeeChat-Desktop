/** @file event_loop.h
 * @brief
 * @author teng.qing
 * @date 2021/7/21
 */

#ifndef MAC_EVENT_LOOP_H_
#define MAC_EVENT_LOOP_H_

#include "cim/base/noncopyable.h"

namespace cim {
namespace base {

/** @class event_loop
 * @brief
 */
class EventLoop : noncopyable {
public:


private:
    EventLoop() = default;
    ~EventLoop() override = default;
};

} // namespace base
} // namespace cim

#endif // MAC_EVENT_LOOP_H_
