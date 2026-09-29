//===-- Helpers to convert WASI filestat to struct stat -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SYS_STAT_WASI_FILESTAT_UTILS_H
#define LLVM_LIBC_SRC_SYS_STAT_WASI_FILESTAT_UTILS_H

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"

#include <sys/stat.h>

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

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

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_SYS_STAT_WASI_FILESTAT_UTILS_H
