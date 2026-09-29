//===-- WASI implementation of dup ----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/dup.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "src/unistd/wasi/dup_utils.h"

namespace LIBC_NAMESPACE_DECL {

// WASI preview1 provides no file-descriptor duplication primitive; emulate
// it by re-opening the recorded path of the descriptor.  See dup_utils.h.
LLVM_LIBC_FUNCTION(int, dup, (int fd)) {
  int newfd = -1;
  int err = wasi::dup_open(fd, &newfd);
  if (err < 0) {
    libc_errno = -err;
    return -1;
  }
  return newfd;
}

} // namespace LIBC_NAMESPACE_DECL
