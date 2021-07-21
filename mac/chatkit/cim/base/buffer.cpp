// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

// Modified : zieckey (zieckey at gmail dot com)

#include "buffer.h"

namespace cim {
namespace base {

const char Buffer::kCRLF[] = "\r\n";

const size_t Buffer::kCheapPrependSize = 8;
const size_t Buffer::kInitialSize = 1024;

} // namespace base
} // namespace cim
