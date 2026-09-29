//===-- WASI implementation of lseek --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/lseek.h"

#include "hdr/stdio_macros.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(off_t, lseek, (int fd, off_t offset, int whence)) {
  wasi::__wasi_whence_t wasi_whence;
  switch (whence) {
  case SEEK_SET:
    wasi_whence = wasi::__WASI_SEEK_SET;
    break;
  case SEEK_CUR:
    wasi_whence = wasi::__WASI_SEEK_CUR;
    break;
  case SEEK_END:
    wasi_whence = wasi::__WASI_SEEK_END;
    break;
  default:
    libc_errno = EINVAL;
    return static_cast<off_t>(-1);
  }

  wasi::__wasi_filesize_t newoffset = 0;
  wasi::__wasi_errno_t err = wasi::__wasi_fd_seek(
      static_cast<wasi::__wasi_fd_t>(fd),
      static_cast<wasi::__wasi_filedelta_t>(offset), wasi_whence,
      &newoffset);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return static_cast<off_t>(-1);
  }
  return static_cast<off_t>(newoffset);
}

} // namespace LIBC_NAMESPACE_DECL
