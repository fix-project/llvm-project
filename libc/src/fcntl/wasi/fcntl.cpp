//===-- WASI implementation of fcntl --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/fcntl/fcntl.h"

#include "src/__support/OSUtil/wasi/fd.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include <stdarg.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, fcntl, (int fd, int cmd, ...)) {
  int arg = 0;
  if (cmd == F_SETFL || cmd == F_SETFD || cmd == F_DUPFD ||
      cmd == F_DUPFD_CLOEXEC) {
    va_list varargs;
    va_start(varargs, cmd);
    arg = va_arg(varargs, int);
    va_end(varargs);
  }

  using namespace wasi;
  switch (cmd) {
  case F_GETFD:
  case F_SETFD: {
    // WASI has no close-on-exec semantics.  Validate the descriptor (POSIX
    // requires EBADF for an invalid fd) and report the flag as always set,
    // matching wasi-libc's F_GETFD return value.
    __wasi_fdstat_t fdstat;
    __wasi_errno_t err = __wasi_fd_fdstat_get(static_cast<__wasi_fd_t>(fd),
                                              &fdstat);
    if (err != __WASI_ERRNO_SUCCESS) {
      libc_errno = wasi_to_errno(err);
      return -1;
    }
    return cmd == F_GETFD ? FD_CLOEXEC : 0;
  }

  case F_GETFL: {
    __wasi_fdstat_t fdstat;
    __wasi_errno_t err = __wasi_fd_fdstat_get(static_cast<__wasi_fd_t>(fd),
                                              &fdstat);
    if (err != __WASI_ERRNO_SUCCESS) {
      libc_errno = wasi_to_errno(err);
      return -1;
    }
    return rights_to_access_mode(fdstat) | fdflags_to_open_flags(fdstat.fs_flags);
  }

  case F_SETFL: {
    __wasi_fdflags_t fdflags = 0;
    if (arg & O_APPEND)
      fdflags |= __WASI_FDFLAGS_APPEND;
    if (arg & O_DSYNC)
      fdflags |= __WASI_FDFLAGS_DSYNC;
    if (arg & O_NONBLOCK)
      fdflags |= __WASI_FDFLAGS_NONBLOCK;
    if (arg & O_RSYNC)
      fdflags |= __WASI_FDFLAGS_RSYNC;
    if (arg & O_SYNC)
      fdflags |= __WASI_FDFLAGS_SYNC;
    __wasi_errno_t err = __wasi_fd_fdstat_set_flags(
        static_cast<__wasi_fd_t>(fd), fdflags);
    if (err != __WASI_ERRNO_SUCCESS) {
      libc_errno = wasi_to_errno(err);
      return -1;
    }
    return 0;
  }

  default:
    // F_DUPFD and F_DUPFD_CLOEXEC are unsupported (WASI preview1 provides no
    // dup primitive); report EINVAL, matching wasi-libc.
    libc_errno = EINVAL;
    return -1;
  }
}

} // namespace LIBC_NAMESPACE_DECL
