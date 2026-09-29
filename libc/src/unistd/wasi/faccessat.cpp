//===-- WASI implementation of faccessat ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/faccessat.h"

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/fcntl_macros.h"
#include "hdr/unistd_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, faccessat,
                   (int fd, const char *path, int amode, int flag)) {
  (void)flag;
  char buf[wasi::PATH_MAX_SIZE];
  auto resolved = wasi::resolve_at(fd, path, buf, sizeof(buf));
  if (!resolved.has_value()) {
    libc_errno = resolved.error();
    return -1;
  }

  size_t len = 0;
  while (resolved->path[len] != '\0')
    ++len;

  // WASI preview1 has no POSIX permission model. Approximate F_OK/R_OK/W_OK
  // by checking existence and attempting to open with the requested rights.
  if (amode & (R_OK | W_OK)) {
    wasi::__wasi_rights_t rights = 0;
    if (amode & R_OK)
      rights |= wasi::__WASI_RIGHT_FD_READ;
    if (amode & W_OK)
      rights |= wasi::__WASI_RIGHT_FD_WRITE | wasi::__WASI_RIGHT_FD_SEEK;
    wasi::__wasi_fd_t opened;
    wasi::__wasi_errno_t err = wasi::__wasi_path_open(
        resolved->dirfd, wasi::__WASI_LOOKUPFLAGS_SYMLINK_FOLLOW,
        resolved->path, static_cast<wasi::__wasi_size_t>(len), 0, rights, 0, 0,
        &opened);
    if (err != wasi::__WASI_ERRNO_SUCCESS) {
      libc_errno = wasi::wasi_to_errno(err);
      return -1;
    }
    wasi::__wasi_fd_close(opened);
    return 0;
  }

  wasi::__wasi_filestat_t statbuf;
  wasi::__wasi_errno_t err = wasi::__wasi_path_filestat_get(
      resolved->dirfd, wasi::__WASI_LOOKUPFLAGS_SYMLINK_FOLLOW, resolved->path,
      static_cast<wasi::__wasi_size_t>(len), &statbuf);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
