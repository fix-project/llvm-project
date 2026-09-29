//===-- Implementation of rename --------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdio/rename.h"

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, rename, (const char *old_path, const char *new_path)) {
  char old_buf[wasi::PATH_MAX_SIZE];
  char new_buf[wasi::PATH_MAX_SIZE];
  auto old_resolved = wasi::resolve_path(old_path, old_buf, sizeof(old_buf));
  if (!old_resolved.has_value()) {
    libc_errno = old_resolved.error();
    return -1;
  }
  auto new_resolved = wasi::resolve_path(new_path, new_buf, sizeof(new_buf));
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

  wasi::__wasi_errno_t err = wasi::__wasi_path_rename(
      old_resolved->dirfd, old_resolved->path,
      static_cast<wasi::__wasi_size_t>(old_len), new_resolved->dirfd,
      new_resolved->path, static_cast<wasi::__wasi_size_t>(new_len));
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
