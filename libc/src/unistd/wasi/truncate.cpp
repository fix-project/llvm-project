//===-- WASI implementation of truncate -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/truncate.h"

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, truncate, (const char *path, off_t length)) {
  char buf[wasi::PATH_MAX_SIZE];
  auto resolved = wasi::resolve_path(path, buf, sizeof(buf));
  if (!resolved.has_value()) {
    libc_errno = resolved.error();
    return -1;
  }

  size_t len = 0;
  while (resolved->path[len] != '\0')
    ++len;

  wasi::__wasi_fd_t fd;
  wasi::__wasi_errno_t err = wasi::__wasi_path_open(
      resolved->dirfd, wasi::__WASI_LOOKUPFLAGS_SYMLINK_FOLLOW, resolved->path,
      static_cast<wasi::__wasi_size_t>(len), 0,
      wasi::__WASI_RIGHT_FD_WRITE | wasi::__WASI_RIGHT_FD_SEEK |
          wasi::__WASI_RIGHT_FD_FILESTAT_SET_SIZE,
      0, 0, &fd);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }

  err = wasi::__wasi_fd_filestat_set_size(
      fd, static_cast<wasi::__wasi_filesize_t>(length));
  wasi::__wasi_fd_close(fd);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
