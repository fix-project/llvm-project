//===-- WASI implementation of posix_fallocate ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Maps directly onto __wasi_fd_allocate, matching wasi-libc.  Following
// POSIX (and wasi-libc), the error number is returned directly rather than
// via libc_errno.
//
//===----------------------------------------------------------------------===//

#include "src/fcntl/posix_fallocate.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"

#include "hdr/errno_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, posix_fallocate, (int fd, off_t offset, off_t len)) {
  if (offset < 0 || len < 0)
    return EINVAL;
  wasi::__wasi_errno_t err = wasi::__wasi_fd_allocate(
      static_cast<wasi::__wasi_fd_t>(fd),
      static_cast<wasi::__wasi_filesize_t>(offset),
      static_cast<wasi::__wasi_filesize_t>(len));
  return err == wasi::__WASI_ERRNO_SUCCESS ? 0 : wasi::wasi_to_errno(err);
}

} // namespace LIBC_NAMESPACE_DECL
