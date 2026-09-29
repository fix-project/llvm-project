//===-- Shared WASI readlink helpers --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_UNISTD_WASI_READLINK_UTILS_H
#define LLVM_LIBC_SRC_UNISTD_WASI_READLINK_UTILS_H

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"

#include "hdr/fcntl_macros.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

LIBC_INLINE ssize_t readlink_impl(int dirfd, const char *__restrict path,
                                  char *__restrict buf, size_t bufsize) {
  char resolved_buf[PATH_MAX];
  auto resolved = resolve_at(dirfd, path, resolved_buf, sizeof(resolved_buf));
  if (!resolved.has_value()) {
    libc_errno = resolved.error();
    return -1;
  }

  size_t len = 0;
  while (resolved->path[len] != '\0')
    ++len;

  __wasi_size_t readlen = 0;
  __wasi_errno_t err =
      __wasi_path_readlink(resolved->dirfd, resolved->path,
                          static_cast<__wasi_size_t>(len), buf,
                          static_cast<__wasi_size_t>(bufsize), &readlen);
  if (err != __WASI_ERRNO_SUCCESS) {
    libc_errno = wasi_to_errno(err);
    return -1;
  }
  return static_cast<ssize_t>(readlen);
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_UNISTD_WASI_READLINK_UTILS_H
