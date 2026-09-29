//===-- Implementation of cfsetspeed --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/termios/cfsetspeed.h"

#include "src/__support/common.h"
#include "src/termios/cfsetispeed.h"
#include "src/termios/cfsetospeed.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, cfsetspeed,
                   (struct termios * termios_p, speed_t speed)) {
  if (LIBC_NAMESPACE::cfsetispeed(termios_p, speed) < 0)
    return -1;
  return LIBC_NAMESPACE::cfsetospeed(termios_p, speed);
}

} // namespace LIBC_NAMESPACE_DECL
