//===-- WASI implementation of stat ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/stat/stat.h"

#include "filestat_utils.h"
#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, stat,
                   (const char *__restrict path,
                    struct stat *__restrict statbuf)) {
  char buf[wasi::PATH_MAX_SIZE];
  auto resolved = wasi::resolve_path(path, buf, sizeof(buf));
  if (!resolved.has_value()) {
    if (wasi::canonicalize_path(path, buf, sizeof(buf), false) == 0 &&
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

  wasi::__wasi_filestat_t wasi_stat;
  wasi::__wasi_errno_t err = wasi::__wasi_path_filestat_get(
      resolved->dirfd, wasi::__WASI_LOOKUPFLAGS_SYMLINK_FOLLOW, resolved->path,
      static_cast<wasi::__wasi_size_t>(len), &wasi_stat);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    if (wasi::is_mount_directory(buf)) {
      wasi::fill_mount_stat(buf, statbuf);
      return 0;
    }
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  wasi::fill_stat(wasi_stat, statbuf);
  statbuf->st_dev = wasi::mount_device(resolved->dirfd);
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
