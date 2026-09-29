//===-- Shared WASI link helpers ------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_UNISTD_WASI_LINK_UTILS_H
#define LLVM_LIBC_SRC_UNISTD_WASI_LINK_UTILS_H

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"

#include "hdr/fcntl_macros.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

LIBC_INLINE int linkat_impl(int olddirfd, const char *oldpath, int newdirfd,
                            const char *newpath, int flags) {
  char old_buf[PATH_MAX];
  auto old_resolved = resolve_at(olddirfd, oldpath, old_buf, sizeof(old_buf));
  if (!old_resolved.has_value()) {
    libc_errno = old_resolved.error();
    return -1;
  }

  char new_buf[PATH_MAX];
  auto new_resolved =
      resolve_at(newdirfd, newpath, new_buf, sizeof(new_buf));
  if (!new_resolved.has_value()) {
    libc_errno = new_resolved.error();
    return -1;
  }

  size_t old_len = 0;
  while (old_resolved->path[old_len] != '\0')
    ++old_len;
  size_t new_len = 0;
  while (new_resolved->path[new_len] != '\0')
    ++new_len;

  // NOTE: wasmtime's preview1 shim returns EINVAL if SYMLINK_FOLLOW is set
  // here, because the underlying preview2 path_link has no follow parameter.
  // Always pass 0.
  (void)flags;
  __wasi_errno_t err =
      __wasi_path_link(old_resolved->dirfd, 0, old_resolved->path,
                      static_cast<__wasi_size_t>(old_len),
                      new_resolved->dirfd, new_resolved->path,
                      static_cast<__wasi_size_t>(new_len));
  if (err != __WASI_ERRNO_SUCCESS) {
    libc_errno = wasi_to_errno(err);
    return -1;
  }
  return 0;
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_UNISTD_WASI_LINK_UTILS_H
