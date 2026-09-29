//===-- WASI implementation of ftruncate ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/ftruncate.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, ftruncate, (int fd, off_t length)) {
  if (length < 0) {
    libc_errno = EINVAL;
    return -1;
  }
  wasi::__wasi_fdstat_t fs;
  wasi::__wasi_errno_t stat_err = wasi::__wasi_fd_fdstat_get(
      static_cast<wasi::__wasi_fd_t>(fd), &fs);
  if (stat_err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(stat_err);
    return -1;
  }
  if (fs.fs_filetype != wasi::__WASI_FILETYPE_REGULAR_FILE ||
      (fs.fs_rights_inheriting & wasi::__WASI_RIGHT_FD_FILESTAT_SET_SIZE) ==
          0) {
    libc_errno = EINVAL;
    return -1;
  }
  wasi::__wasi_errno_t err = wasi::__wasi_fd_filestat_set_size(
      static_cast<wasi::__wasi_fd_t>(fd),
      static_cast<wasi::__wasi_filesize_t>(length));
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
