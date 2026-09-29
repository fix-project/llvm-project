//===-- WASIp1 implementation of system -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdlib/system.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"

#include <errno.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, system, (const char *command)) {
  if (command == nullptr)
    return 0;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
