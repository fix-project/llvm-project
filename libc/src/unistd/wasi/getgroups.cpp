//===-- WASI implementation of getgroups ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/getgroups.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, getgroups, (int size, gid_t *list)) {
  (void)list;
  if (size < 0) {
    libc_errno = EINVAL;
    return -1;
  }
  // WASIp1 has no supplementary group IDs.
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
