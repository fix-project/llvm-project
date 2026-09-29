//===-- Implementation of remove --------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdio/remove.h"

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, remove, (const char *path)) {
  char buf[wasi::PATH_MAX_SIZE];
  auto resolved = wasi::resolve_path(path, buf, sizeof(buf));
  if (!resolved.has_value()) {
    libc_errno = resolved.error();
    return -1;
  }

  size_t len = 0;
  while (resolved->path[len] != '\0')
    ++len;

  wasi::__wasi_errno_t err = wasi::__wasi_path_unlink_file(
      resolved->dirfd, resolved->path, static_cast<wasi::__wasi_size_t>(len));
  if (err == wasi::__WASI_ERRNO_SUCCESS)
    return 0;
  if (err != wasi::__WASI_ERRNO_ISDIR && err != wasi::__WASI_ERRNO_NOTDIR) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }

  // Not a regular file; try removing it as a directory.
  err = wasi::__wasi_path_remove_directory(
      resolved->dirfd, resolved->path, static_cast<wasi::__wasi_size_t>(len));
  if (err == wasi::__WASI_ERRNO_SUCCESS)
    return 0;

  libc_errno = wasi::wasi_to_errno(err);
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
