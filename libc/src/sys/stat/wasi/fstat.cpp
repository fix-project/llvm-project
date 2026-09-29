//===-- WASI implementation of fstat --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/stat/fstat.h"

#include "filestat_utils.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, fstat, (int fd, struct stat *statbuf)) {
  wasi::__wasi_filestat_t wasi_stat;
  wasi::__wasi_errno_t err = wasi::__wasi_fd_filestat_get(
      static_cast<wasi::__wasi_fd_t>(fd), &wasi_stat);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  wasi::fill_stat(wasi_stat, statbuf);
  wasi::assign_mount_device(fd, statbuf);
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
