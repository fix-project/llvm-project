//===-- WASI implementation of pread --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/pread.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, pread,
                   (int fd, void *buf, size_t count, off_t offset)) {
  wasi::__wasi_iovec_t iov = {buf, static_cast<wasi::__wasi_size_t>(count)};
  wasi::__wasi_size_t nread = 0;
  wasi::__wasi_errno_t err = wasi::__wasi_fd_pread(
      static_cast<wasi::__wasi_fd_t>(fd), &iov, 1,
      static_cast<wasi::__wasi_filesize_t>(offset), &nread);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  return static_cast<ssize_t>(nread);
}

} // namespace LIBC_NAMESPACE_DECL
