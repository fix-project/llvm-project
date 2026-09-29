//===-- WASI implementation of close --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/close.h"

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "src/sys/epoll/wasi/epoll_state.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, close, (int fd)) {
  if (wasi::epoll_release(fd)) {
    libc_errno = 0;
    return 0;
  }
  wasi::__wasi_errno_t err =
      wasi::__wasi_fd_close(static_cast<wasi::__wasi_fd_t>(fd));
  wasi::unregister_fd_path(fd);
  wasi::unmark_fd_o_path(fd);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  libc_errno = 0;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
