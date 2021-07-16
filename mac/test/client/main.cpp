/** @file main.h
  * @brief 
  * @author teng.qing
  * @date 2021/7/16
  */

#include <iostream>

using namespace std;

#include "cim/cim.h"

int main() {
    cout << "hello world" << endl;

    cim::core::Client::getInstance()->login()

    return 0;
}