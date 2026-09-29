//===-- WASI implementation of fstatvfs -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/statvfs/fstatvfs.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, fstatvfs, (int fd, struct statvfs *buf)) {
  if (buf == nullptr) {
    libc_errno = EFAULT;
    return -1;
  }

  wasi::__wasi_filestat_t statbuf;
  wasi::__wasi_errno_t err = wasi::__wasi_fd_filestat_get(
      static_cast<wasi::__wasi_fd_t>(fd), &statbuf);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }

  constexpr unsigned long BLOCK_SIZE = 4096;
  constexpr unsigned long FAKE_FREE_BLOCKS = 65536;

  buf->f_bsize = BLOCK_SIZE;
  buf->f_frsize = BLOCK_SIZE;
  buf->f_blocks =
      static_cast<fsblkcnt_t>(statbuf.st_size / BLOCK_SIZE) + FAKE_FREE_BLOCKS;
  buf->f_bfree = FAKE_FREE_BLOCKS;
  buf->f_bavail = FAKE_FREE_BLOCKS;
  buf->f_files = 0;
  buf->f_ffree = 0;
  buf->f_favail = 0;
  buf->f_fsid = 0;
  buf->f_flag = 0;
  buf->f_namemax = 255;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
