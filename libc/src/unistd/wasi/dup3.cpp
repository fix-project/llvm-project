//===-- WASI implementation of dup3 ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/dup3.h"

#include "hdr/fcntl_macros.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "src/unistd/wasi/dup_utils.h"

namespace LIBC_NAMESPACE_DECL {

// Emulated like dup2(); O_CLOEXEC carries no state on WASI since there is
// no exec to clear it on.  See dup_utils.h.
LLVM_LIBC_FUNCTION(int, dup3, (int oldfd, int newfd, int flags)) {
  if (oldfd == newfd) {
    libc_errno = EINVAL;
    return -1;
  }
  if ((flags & ~O_CLOEXEC) != 0) {
    libc_errno = EINVAL;
    return -1;
  }
  if (newfd < 0) {
    libc_errno = EBADF;
    return -1;
  }

  int result = wasi::dup_to(oldfd, newfd);
  if (result < 0) {
    libc_errno = -result;
    return -1;
  }
  return result;
}

} // namespace LIBC_NAMESPACE_DECL
