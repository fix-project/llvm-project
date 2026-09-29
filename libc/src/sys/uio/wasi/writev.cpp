//===-- WASI implementation of writev -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/uio/writev.h"

#include "hdr/types/ssize_t.h"
#include "hdr/types/struct_iovec.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, writev, (int fd, const iovec *iov, int iovcnt)) {
  if (iovcnt < 0) {
    libc_errno = EINVAL;
    return -1;
  }
  wasi::__wasi_size_t nwritten = 0;
  wasi::__wasi_errno_t err = wasi::__wasi_fd_write(
      static_cast<wasi::__wasi_fd_t>(fd),
      reinterpret_cast<const wasi::__wasi_ciovec_t *>(iov),
      static_cast<wasi::__wasi_size_t>(iovcnt), &nwritten);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  return static_cast<ssize_t>(nwritten);
}

} // namespace LIBC_NAMESPACE_DECL
