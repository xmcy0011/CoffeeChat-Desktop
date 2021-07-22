/** @file user_manager_test.h
 * @brief
 * @author teng.qing
 * @date 2021/7/22
 */

#include "cim/cim.h"
#include "gtest/gtest.h"

using namespace cim::core;

// UserManager/registerUser
TEST(UserManager, registerUser) {
    HttpResponseBase res;
    ASSERT_TRUE(UserManager::getInstance()->registerUser("gaozz", "123456", "高真真", res));
}