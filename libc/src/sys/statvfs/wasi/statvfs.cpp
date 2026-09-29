//===-- WASI implementation of statvfs ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/statvfs/statvfs.h"

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, statvfs,
                   (const char *__restrict path, struct statvfs *__restrict buf)) {
  if (buf == nullptr) {
    libc_errno = EFAULT;
    return -1;
  }

  char pathbuf[wasi::PATH_MAX_SIZE];
  auto resolved = wasi::resolve_path(path, pathbuf, sizeof(pathbuf));
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
      wasi::__WASI_RIGHT_FD_FILESTAT_GET, 0, 0, &fd);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }

  wasi::__wasi_filestat_t statbuf;
  err = wasi::__wasi_fd_filestat_get(fd, &statbuf);
  wasi::__wasi_fd_close(fd);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }

  constexpr unsigned long BLOCK_SIZE = 4096;
  constexpr unsigned long FAKE_FREE_BLOCKS = 65536;

  buf->f_bsize = BLOCK_SIZE;
  buf->f_frsize = BLOCK_SIZE;
  buf->f_blocks =
      static_cast<fsblkcnt_t>(statbuf.st_size / BLOCK_SIZE) + FAKE_FREE_BLOCKS;
  buf->f_bfree = FAKE_FREE_BLOCKS;
  buf->f_bavail = FAKE_FREE_BLOCKS;
  buf->f_files = 0;
  buf->f_ffree = 0;
  buf->f_favail = 0;
  buf->f_fsid = 0;
  buf->f_flag = 0;
  buf->f_namemax = 255;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
