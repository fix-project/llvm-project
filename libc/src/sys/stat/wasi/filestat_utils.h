//===-- Helpers to convert WASI filestat to struct stat -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SYS_STAT_WASI_FILESTAT_UTILS_H
#define LLVM_LIBC_SRC_SYS_STAT_WASI_FILESTAT_UTILS_H

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"

#include <sys/stat.h>

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

// A preopen is a distinct mount in the guest namespace, even when two
// preopens happen to reside on the same host filesystem.
LIBC_INLINE dev_t mount_device(__wasi_fd_t fd) {
  return (dev_t(1) << 63) | static_cast<dev_t>(fd + 1);
}

LIBC_INLINE void assign_mount_device(int fd, struct stat *dst) {
  __wasi_prestat_t prestat;
  if (__wasi_fd_prestat_get(static_cast<__wasi_fd_t>(fd), &prestat) ==
      __WASI_ERRNO_SUCCESS) {
    dst->st_dev = mount_device(static_cast<__wasi_fd_t>(fd));
    return;
  }
  char path[PATH_MAX_SIZE];
  if (!lookup_fd_path(fd, path, sizeof(path)))
    return;
  char resolved_path[PATH_MAX_SIZE];
  auto resolved = resolve_path(path, resolved_path, sizeof(resolved_path));
  if (resolved.has_value())
    dst->st_dev = mount_device(resolved->dirfd);
}

LIBC_INLINE mode_t filetype_to_mode(__wasi_filetype_t filetype) {
  switch (filetype) {
  case __WASI_FILETYPE_DIRECTORY:
    return S_IFDIR | 0777;
  case __WASI_FILETYPE_REGULAR_FILE:
    return S_IFREG | 0666;
  case __WASI_FILETYPE_CHARACTER_DEVICE:
    return S_IFCHR | 0666;
  case __WASI_FILETYPE_BLOCK_DEVICE:
    return S_IFBLK | 0666;
  case __WASI_FILETYPE_SYMBOLIC_LINK:
    return S_IFLNK | 0777;
  case __WASI_FILETYPE_SOCKET_DGRAM:
  case __WASI_FILETYPE_SOCKET_STREAM:
    return S_IFSOCK | 0777;
  default:
    return S_IFREG | 0666;
  }
}

LIBC_INLINE void fill_stat(const __wasi_filestat_t &src, struct stat *dst) {
  dst->st_dev = static_cast<dev_t>(src.st_dev);
  dst->st_ino = static_cast<ino_t>(src.st_ino);
  dst->st_mode = filetype_to_mode(src.st_filetype);
  dst->st_nlink = static_cast<nlink_t>(src.st_nlink);
  dst->st_uid = 0;
  dst->st_gid = 0;
  dst->st_rdev = 0;
  dst->st_size = static_cast<off_t>(src.st_size);
  dst->st_atim.tv_sec = static_cast<time_t>(src.st_atim / 1000000000ULL);
  dst->st_atim.tv_nsec = static_cast<long>(src.st_atim % 1000000000ULL);
  dst->st_mtim.tv_sec = static_cast<time_t>(src.st_mtim / 1000000000ULL);
  dst->st_mtim.tv_nsec = static_cast<long>(src.st_mtim % 1000000000ULL);
  dst->st_ctim.tv_sec = static_cast<time_t>(src.st_ctim / 1000000000ULL);
  dst->st_ctim.tv_nsec = static_cast<long>(src.st_ctim % 1000000000ULL);
  dst->st_blksize = 4096;
  dst->st_blocks = static_cast<blkcnt_t>((src.st_size + 511) / 512);
}

// Synthetic mount ancestors must have distinct identities. Directory walkers
// use (st_dev, st_ino) to detect cycles, so giving every ancestor zero for
// both fields would hide nested preopens.
LIBC_INLINE void fill_mount_stat(const char *path, struct stat *dst) {
  uint64_t inode = 14695981039346656037ULL;
  for (const unsigned char *p = reinterpret_cast<const unsigned char *>(path);
       *p; ++p)
    inode = (inode ^ *p) * 1099511628211ULL;
  __wasi_filestat_t virtual_dir = {};
  virtual_dir.st_dev = ~uint64_t(0);
  virtual_dir.st_ino = inode;
  virtual_dir.st_filetype = __WASI_FILETYPE_DIRECTORY;
  virtual_dir.st_nlink = 1;
  fill_stat(virtual_dir, dst);
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_SYS_STAT_WASI_FILESTAT_UTILS_H
