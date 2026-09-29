//===-- WASI helpers for dup/dup2/dup3 ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// WASI preview1 has no descriptor duplication primitive, and runtimes
/// (e.g. wasmtime) additionally reject `fd_renumber` targets that are not
/// currently open.  For descriptors opened through this libc (and therefore
/// recorded in the fd-to-path registry) duplication is emulated by
/// re-opening the recorded path with the recorded flags; `fd` allocates the
/// lowest free descriptor, matching POSIX `dup()`.  For dup2()/dup3() the
/// target is first closed and the file is re-opened until the allocator
/// lands on the requested descriptor number.  Descriptors that were not
/// opened through this libc (stdin/stdout/stderr, descriptors created by
/// the runtime, ...) cannot be duplicated and yield ENOSYS (wasi-libc does
/// not provide dup/dup2/dup3 at all).
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_UNISTD_WASI_DUP_UTILS_H
#define LLVM_LIBC_SRC_UNISTD_WASI_DUP_UTILS_H

#include "hdr/errno_macros.h"
#include "hdr/fcntl_macros.h"
#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"
#include "src/fcntl/wasi/open_utils.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

// Maximum number of temporary descriptors held open while steering the
// allocator onto a specific descriptor number.
constexpr int DUP_TO_MAX_HOP = 4096;

// Returns 0 when `fd` refers to an open descriptor, -EBADF otherwise.
LIBC_INLINE int validate_fd(int fd) {
  if (fd < 0)
    return -EBADF;
  __wasi_fdstat_t st;
  if (__wasi_fd_fdstat_get(static_cast<__wasi_fd_t>(fd), &st) !=
      __WASI_ERRNO_SUCCESS)
    return -EBADF;
  return 0;
}

// Opens a fresh descriptor referring to the same file as `oldfd`.  Returns
// 0 and stores the new descriptor in `*out_fd` on success, or a negative
// errno on failure.
LIBC_INLINE int dup_open(int oldfd, int *out_fd) {
  int err = validate_fd(oldfd);
  if (err < 0)
    return err;

  char path[PATH_MAX];
  int flags = 0;
  if (!fd_dup_info(oldfd, path, sizeof(path), &flags))
    return -ENOSYS;

  // Creation-only flags must not be replayed when duplicating.
  flags &= ~(O_CREAT | O_EXCL | O_TRUNC);

  int fd = openat_impl(AT_FDCWD, path, flags, 0);
  if (fd < 0)
    return fd;

  *out_fd = fd;
  return 0;
}

// Closes `fd` including all bookkeeping, ignoring whether it was open.
LIBC_INLINE void force_close(int fd) {
  unmark_fd_o_path(fd);
  unregister_fd_path(fd);
  __wasi_fd_close(static_cast<__wasi_fd_t>(fd));
}

// Duplicates `oldfd` onto the exact descriptor number `newfd`, closing
// whatever was previously open as `newfd`.  Returns `newfd` on success or a
// negative errno on failure.
LIBC_INLINE int dup_to(int oldfd, int newfd) {
  int err = validate_fd(oldfd);
  if (err < 0)
    return err;

  char path[PATH_MAX];
  int flags = 0;
  if (!fd_dup_info(oldfd, path, sizeof(path), &flags))
    return -ENOSYS;

  // Creation-only flags must not be replayed when duplicating.
  flags &= ~(O_CREAT | O_EXCL | O_TRUNC);

  // POSIX dup2() closes the target first; doing so also guarantees that
  // the descriptor allocator can eventually reach `newfd`.
  force_close(newfd);

  int held[DUP_TO_MAX_HOP];
  int held_n = 0;
  int result = -EBADF;
  for (;;) {
    int fd = openat_impl(AT_FDCWD, path, flags, 0);
    if (fd < 0) {
      result = fd;
      break;
    }
    if (fd == newfd) {
      result = fd;
      break;
    }
    if (fd > newfd || held_n >= DUP_TO_MAX_HOP) {
      force_close(fd);
      result = -EMFILE;
      break;
    }
    held[held_n++] = fd;
  }

  for (int i = 0; i < held_n; ++i)
    force_close(held[i]);

  return result;
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_UNISTD_WASI_DUP_UTILS_H
