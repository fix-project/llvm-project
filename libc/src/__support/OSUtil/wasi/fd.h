//===-- WASI file descriptor helpers --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Helpers translating between POSIX open/flag representations (O_*,
/// F_*) and the corresponding WASI flag, right and fdflag sets.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_FD_H
#define LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_FD_H

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/error_or.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"

#include "hdr/fcntl_macros.h"

#include "hdr/errno_macros.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

// The WASI parameters corresponding to a POSIX open(2) call.
struct WasiOpenFlags {
  __wasi_oflags_t oflags;
  __wasi_rights_t rights_base;
  __wasi_rights_t rights_inheriting;
  __wasi_fdflags_t fdflags;
};

// Translate POSIX O_* flags into a WasiOpenFlags structure. The rights of a
// newly opened descriptor cannot exceed the parent directory's inheriting
// rights. Returns EINVAL for access modes that cannot be expressed on WASI.
LIBC_INLINE ErrorOr<WasiOpenFlags>
open_flags_to_wasi(int flags, __wasi_rights_t parent_inheriting_rights) {
  WasiOpenFlags result = {};

  int accmode = flags & O_ACCMODE;

  // O_SEARCH and O_EXEC are aliases for path lookup / program execution
  // permissions; treat them as execute-only access.
  if (accmode == O_EXEC || accmode == O_SEARCH || (flags & O_PATH)) {
    // WASI has no notion of execute-only or path-only references; grant a
    // minimal set of rights.
  } else {
    if (accmode == O_RDONLY || accmode == O_RDWR)
      result.rights_base |=
          __WASI_RIGHT_FD_READ | __WASI_RIGHT_FD_SEEK | __WASI_RIGHT_FD_TELL |
          __WASI_RIGHT_FD_FILESTAT_GET | __WASI_RIGHT_FD_READDIR;
    if (accmode == O_WRONLY || accmode == O_RDWR)
      result.rights_base |= __WASI_RIGHT_FD_WRITE | __WASI_RIGHT_FD_SEEK |
                            __WASI_RIGHT_FD_TELL |
                            __WASI_RIGHT_FD_FILESTAT_GET |
                            __WASI_RIGHT_FD_FILESTAT_SET_SIZE |
                            __WASI_RIGHT_FD_FILESTAT_SET_TIMES |
                            __WASI_RIGHT_FD_SYNC | __WASI_RIGHT_FD_DATASYNC |
                            __WASI_RIGHT_FD_ALLOCATE;
  }

  if (flags & O_CREAT)
    result.oflags |= __WASI_OFLAGS_CREAT;
  if (flags & O_EXCL)
    result.oflags |= __WASI_OFLAGS_EXCL;
  if (flags & O_TRUNC)
    result.oflags |= __WASI_OFLAGS_TRUNC;
  if (flags & O_DIRECTORY)
    result.oflags |= __WASI_OFLAGS_DIRECTORY;

  if (flags & O_APPEND)
    result.fdflags |= __WASI_FDFLAGS_APPEND;
  if (flags & O_DSYNC)
    result.fdflags |= __WASI_FDFLAGS_DSYNC;
  if (flags & O_NONBLOCK)
    result.fdflags |= __WASI_FDFLAGS_NONBLOCK;
  if (flags & O_RSYNC)
    result.fdflags |= __WASI_FDFLAGS_RSYNC;
  if (flags & O_SYNC)
    result.fdflags |= __WASI_FDFLAGS_SYNC;

  // Directories need the path-scoped rights to be usable as a directory
  // file descriptor (e.g. for openat, readdir, mkdirat, ...).
  constexpr __wasi_rights_t path_rights =
      __WASI_RIGHT_PATH_CREATE_FILE |
      __WASI_RIGHT_PATH_CREATE_DIRECTORY | __WASI_RIGHT_PATH_REMOVE_DIRECTORY |
      __WASI_RIGHT_PATH_OPEN | __WASI_RIGHT_PATH_RENAME_SOURCE |
      __WASI_RIGHT_PATH_RENAME_TARGET | __WASI_RIGHT_PATH_FILESTAT_GET |
      __WASI_RIGHT_PATH_FILESTAT_SET_SIZE |
      __WASI_RIGHT_PATH_FILESTAT_SET_TIMES | __WASI_RIGHT_PATH_SYMLINK |
      __WASI_RIGHT_PATH_LINK_SOURCE | __WASI_RIGHT_PATH_LINK_TARGET |
      __WASI_RIGHT_PATH_READLINK | __WASI_RIGHT_PATH_UNLINK_FILE;
  // The runtime drops rights that do not apply to regular files. Directories
  // need path rights in their base set for operations relative to their fd.
  result.rights_base =
      (result.rights_base | path_rights) & parent_inheriting_rights;

  // Inheriting rights describe what can be granted to descriptors opened
  // through this one, independently of this descriptor's access mode. A
  // path_open cannot create a socket, so never request socket rights.
  result.rights_inheriting =
      parent_inheriting_rights &
      ~(__WASI_RIGHT_SOCK_SHUTDOWN | __WASI_RIGHT_SOCK_ACCEPT);

  return result;
}

// Translate WASI fdflags back into the POSIX O_* flags they imply.
LIBC_INLINE int fdflags_to_open_flags(__wasi_fdflags_t fdflags) {
  int flags = 0;
  if (fdflags & __WASI_FDFLAGS_APPEND)
    flags |= O_APPEND;
  if (fdflags & __WASI_FDFLAGS_DSYNC)
    flags |= O_DSYNC;
  if (fdflags & __WASI_FDFLAGS_NONBLOCK)
    flags |= O_NONBLOCK;
  if (fdflags & __WASI_FDFLAGS_RSYNC)
    flags |= O_RSYNC;
  if (fdflags & __WASI_FDFLAGS_SYNC)
    flags |= O_SYNC;
  return flags;
}

// Derive the POSIX access mode from WASI rights.
LIBC_INLINE int rights_to_access_mode(const __wasi_fdstat_t &fdstat) {
  bool can_read = (fdstat.fs_rights_base &
                   (__WASI_RIGHT_FD_READ | __WASI_RIGHT_FD_READDIR)) != 0;
  bool can_write = (fdstat.fs_rights_base & __WASI_RIGHT_FD_WRITE) != 0;
  if (can_read && can_write)
    return O_RDWR;
  if (can_write)
    return O_WRONLY;
  if (can_read)
    return O_RDONLY;
  return O_SEARCH;
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_FD_H
