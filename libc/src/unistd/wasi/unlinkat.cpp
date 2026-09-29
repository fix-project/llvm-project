//===-- WASI implementation of unlinkat -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/unlinkat.h"

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/fcntl_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, unlinkat, (int dirfd, const char *path, int flags)) {
  char buf[wasi::PATH_MAX_SIZE];
  auto resolved = wasi::resolve_at(dirfd, path, buf, sizeof(buf));
  if (!resolved.has_value()) {
    libc_errno = resolved.error();
    return -1;
  }

  size_t len = 0;
  while (resolved->path[len] != '\0')
    ++len;

  wasi::__wasi_errno_t err;
  if (flags & AT_REMOVEDIR)
    err = wasi::__wasi_path_remove_directory(
        resolved->dirfd, resolved->path, static_cast<wasi::__wasi_size_t>(len));
  else
    err = wasi::__wasi_path_unlink_file(
        resolved->dirfd, resolved->path, static_cast<wasi::__wasi_size_t>(len));

  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
