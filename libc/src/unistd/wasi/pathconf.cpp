//===-- WASI implementation of pathconf -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/pathconf.h"

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/unistd_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(long, pathconf, (const char *path, int name)) {
  char buf[wasi::PATH_MAX_SIZE];
  auto resolved = wasi::resolve_path(path, buf, sizeof(buf));
  if (!resolved.has_value()) {
    libc_errno = resolved.error();
    return -1;
  }

  size_t len = 0;
  while (resolved->path[len] != '\0')
    ++len;

  wasi::__wasi_filestat_t statbuf;
  wasi::__wasi_errno_t err = wasi::__wasi_path_filestat_get(
      resolved->dirfd, wasi::__WASI_LOOKUPFLAGS_SYMLINK_FOLLOW, resolved->path,
      static_cast<wasi::__wasi_size_t>(len), &statbuf);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }

  switch (name) {
  case _PC_LINK_MAX:
    return 1;
  case _PC_NAME_MAX:
    return 255;
  case _PC_PATH_MAX:
    return 256;
  case _PC_SYMLINK_MAX:
    return 255;
  case _PC_NO_TRUNC:
    return 1;
  case _PC_CHOWN_RESTRICTED:
    return 0;
  case _PC_FILESIZEBITS:
    return 64;
  case _PC_2_SYMLINKS:
    return 1;
  default:
    libc_errno = EINVAL;
    return -1;
  }
}

} // namespace LIBC_NAMESPACE_DECL
