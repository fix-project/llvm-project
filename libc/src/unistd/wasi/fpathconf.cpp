//===-- WASI implementation of fpathconf ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/fpathconf.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/unistd_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(long, fpathconf, (int fd, int name)) {
  wasi::__wasi_fdstat_t fdstat;
  wasi::__wasi_errno_t err =
      wasi::__wasi_fd_fdstat_get(static_cast<wasi::__wasi_fd_t>(fd), &fdstat);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }

  switch (name) {
  case _PC_LINK_MAX:
    return 1;
  case _PC_NAME_MAX:
    return 255;
  case _PC_PATH_MAX:
    return 256;
  case _PC_SYMLINK_MAX:
    return 255;
  case _PC_NO_TRUNC:
    return 1;
  case _PC_CHOWN_RESTRICTED:
    return 0;
  case _PC_FILESIZEBITS:
    return 64;
  case _PC_2_SYMLINKS:
    return 1;
  default:
    libc_errno = EINVAL;
    return -1;
  }
}

} // namespace LIBC_NAMESPACE_DECL
