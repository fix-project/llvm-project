//===-- Shared WASI open helpers ------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Shared implementation of the openat(2) semantics used by open(),
/// openat() and creat().
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_FCNTL_WASI_OPEN_UTILS_H
#define LLVM_LIBC_SRC_FCNTL_WASI_OPEN_UTILS_H

#include "src/__support/OSUtil/wasi/fd.h"
#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"

#include "hdr/types/mode_t.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

LIBC_INLINE size_t open_str_len(const char *s) {
  size_t n = 0;
  while (s[n] != '\0')
    ++n;
  return n;
}

// Returns the lowest descriptor number that is not currently open, or -1 if
// every descriptor below the probe limit is in use.
LIBC_INLINE int lowest_free_fd() {
  for (int fd = 0; fd < (1 << 20); ++fd) {
    __wasi_fdstat_t st;
    if (__wasi_fd_fdstat_get(static_cast<__wasi_fd_t>(fd), &st) ==
        __WASI_ERRNO_BADF)
      return fd;
  }
  return -1;
}

LIBC_INLINE int openat_impl(int dirfd, const char *path, int flags,
                            mode_t mode) {
  (void)mode; // WASI preview1 has no permission model.

  char buf[PATH_MAX];
  auto resolved = resolve_at(dirfd, path, buf, sizeof(buf));
  if (!resolved.has_value())
    return -resolved.error();

  auto converted = open_flags_to_wasi(flags);
  if (!converted.has_value())
    return -converted.error();
  WasiOpenFlags wf = converted.value();

  __wasi_lookupflags_t lookup =
      (flags & O_NOFOLLOW) ? 0 : __WASI_LOOKUPFLAGS_SYMLINK_FOLLOW;

  // POSIX requires open() to return the lowest-numbered unused descriptor,
  // but WASI runtimes (e.g. wasmtime) are free to pick any free descriptor.
  // If the runtime hands back a descriptor above the lowest free one, keep
  // re-opening the file (holding the extra descriptors open so the runtime
  // cannot reuse them) until the allocation lands on the lowest free slot.
  const int target = lowest_free_fd();
  int held[1024];
  int held_n = 0;
  int fd = -1;
  for (;;) {
    __wasi_fd_t raw_fd;
    __wasi_errno_t err =
        __wasi_path_open(resolved->dirfd, lookup, resolved->path,
                         open_str_len(resolved->path), wf.oflags,
                         wf.rights_base, wf.rights_inheriting, wf.fdflags,
                         &raw_fd);
    if (err != __WASI_ERRNO_SUCCESS) {
      for (int i = 0; i < held_n; ++i)
        __wasi_fd_close(static_cast<__wasi_fd_t>(held[i]));
      return -wasi_to_errno(err);
    }
    fd = static_cast<int>(raw_fd);
    if (target < 0 || fd == target)
      break;
    if (held_n >= 1024) // Should not happen; fall back to what we have.
      break;
    held[held_n++] = fd;
    // The file now exists; creation flags must not be replayed.
    converted = open_flags_to_wasi(flags & ~(O_CREAT | O_EXCL | O_TRUNC));
    if (!converted.has_value())
      break;
    wf = converted.value();
  }

  for (int i = 0; i < held_n; ++i)
    __wasi_fd_close(static_cast<__wasi_fd_t>(held[i]));

  if (flags & O_PATH)
    mark_fd_o_path(fd);
  // Track the absolute path and open flags of every opened descriptor so
  // that fchdir() and dup() can be emulated.
  if (buf[0] == '/')
    register_fd(fd, buf, flags);
  return fd;
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_FCNTL_WASI_OPEN_UTILS_H
