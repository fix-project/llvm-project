//===-- WASI implementation of fstatat -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/stat/fstatat.h"

#include "filestat_utils.h"
#include "hdr/fcntl_macros.h"
#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, fstatat,
                   (int dirfd, const char *path, struct stat *statbuf,
                    int flags)) {
  if (flags & ~AT_SYMLINK_NOFOLLOW) {
    libc_errno = EINVAL;
    return -1;
  }
  char buf[wasi::PATH_MAX_SIZE];
  bool absolute_name = dirfd == AT_FDCWD || (path && path[0] == '/');
  auto resolved = wasi::resolve_at(dirfd, path, buf, sizeof(buf));
  if (!resolved.has_value()) {
    if (absolute_name &&
        wasi::canonicalize_path(path, buf, sizeof(buf), false) == 0 &&
        wasi::is_mount_directory(buf)) {
      wasi::fill_mount_stat(buf, statbuf);
      return 0;
    }
    libc_errno = resolved.error();
    return -1;
  }
  size_t len = 0;
  while (resolved->path[len] != '\0')
    ++len;
  wasi::__wasi_filestat_t result;
  wasi::__wasi_lookupflags_t lookup =
      (flags & AT_SYMLINK_NOFOLLOW) ? 0
                                    : wasi::__WASI_LOOKUPFLAGS_SYMLINK_FOLLOW;
  wasi::__wasi_errno_t err = wasi::__wasi_path_filestat_get(
      resolved->dirfd, lookup, resolved->path,
      static_cast<wasi::__wasi_size_t>(len), &result);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    if (absolute_name && wasi::is_mount_directory(buf)) {
      wasi::fill_mount_stat(buf, statbuf);
      return 0;
    }
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  wasi::fill_stat(result, statbuf);
  if (absolute_name)
    statbuf->st_dev = wasi::mount_device(resolved->dirfd);
  else
    wasi::assign_mount_device(dirfd, statbuf);
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
