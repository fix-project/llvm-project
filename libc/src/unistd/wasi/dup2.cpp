//===-- WASI implementation of dup2 ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/dup2.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "src/unistd/wasi/dup_utils.h"

namespace LIBC_NAMESPACE_DECL {

// Emulated by re-opening the recorded path of the descriptor and moving the
// result onto `newfd` with fd_renumber().  See dup_utils.h.
LLVM_LIBC_FUNCTION(int, dup2, (int oldfd, int newfd)) {
  if (newfd < 0) {
    libc_errno = EBADF;
    return -1;
  }

  if (oldfd == newfd) {
    int err = wasi::validate_fd(oldfd);
    if (err < 0) {
      libc_errno = -err;
      return -1;
    }
    return newfd;
  }

  int result = wasi::dup_to(oldfd, newfd);
  if (result < 0) {
    libc_errno = -result;
    return -1;
  }
  return result;
}

} // namespace LIBC_NAMESPACE_DECL
