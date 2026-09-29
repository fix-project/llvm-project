//===-- Shared WASI symlink helpers ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_UNISTD_WASI_SYMLINK_UTILS_H
#define LLVM_LIBC_SRC_UNISTD_WASI_SYMLINK_UTILS_H

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"

#include "hdr/fcntl_macros.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

LIBC_INLINE int symlinkat_impl(const char *target, int newdirfd,
                               const char *linkpath) {
  char buf[PATH_MAX];
  auto resolved = resolve_at(newdirfd, linkpath, buf, sizeof(buf));
  if (!resolved.has_value()) {
    libc_errno = resolved.error();
    return -1;
  }

  size_t target_len = 0;
  while (target[target_len] != '\0')
    ++target_len;
  size_t link_len = 0;
  while (resolved->path[link_len] != '\0')
    ++link_len;

  __wasi_errno_t err =
      __wasi_path_symlink(target, static_cast<__wasi_size_t>(target_len),
                          resolved->dirfd, resolved->path,
                          static_cast<__wasi_size_t>(link_len));
  if (err != __WASI_ERRNO_SUCCESS) {
    libc_errno = wasi_to_errno(err);
    return -1;
  }
  return 0;
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_UNISTD_WASI_SYMLINK_UTILS_H
