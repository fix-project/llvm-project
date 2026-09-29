//===-- WASI implementation of isatty -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/isatty.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, isatty, (int fd)) {
  wasi::__wasi_fdstat_t statbuf;
  wasi::__wasi_errno_t err = wasi::__wasi_fd_fdstat_get(
      static_cast<wasi::__wasi_fd_t>(fd), &statbuf);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return 0;
  }
  if (statbuf.fs_filetype != wasi::__WASI_FILETYPE_CHARACTER_DEVICE) {
    libc_errno = ENOTTY;
    return 0;
  }
  return 1;
}

} // namespace LIBC_NAMESPACE_DECL
