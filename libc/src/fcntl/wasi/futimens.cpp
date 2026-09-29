//===-- WASI implementation of futimens -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/fcntl/futimens.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/sys_stat_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, futimens, (int fd, const struct timespec times[2])) {
  using namespace wasi;
  __wasi_timestamp_t atim = __WASI_FTIMES_SPECIAL_OMIT;
  __wasi_timestamp_t mtim = __WASI_FTIMES_SPECIAL_OMIT;
  __wasi_fstflags_t fstflags = 0;

  if (times == nullptr) {
    fstflags = __WASI_FILESTAT_SET_ATIM_NOW | __WASI_FILESTAT_SET_MTIM_NOW;
  } else {
    for (int i = 0; i < 2; ++i) {
      long long tv_nsec = times[i].tv_nsec;
      if (tv_nsec == UTIME_OMIT)
        continue;
      bool now = tv_nsec == UTIME_NOW;
      if (!now && (tv_nsec < 0 || tv_nsec >= 1000000000 ||
                   times[i].tv_sec < 0)) {
        libc_errno = EINVAL;
        return -1;
      }
      __wasi_timestamp_t ts =
          now ? 0 : static_cast<__wasi_timestamp_t>(times[i].tv_sec) *
                        UINT64_C(1000000000) +
                        static_cast<__wasi_timestamp_t>(tv_nsec);
      if (i == 0)
        atim = ts;
      else
        mtim = ts;
      fstflags |= now ? (i == 0 ? __WASI_FILESTAT_SET_ATIM_NOW
                              : __WASI_FILESTAT_SET_MTIM_NOW)
                      : (i == 0 ? __WASI_FILESTAT_SET_ATIM
                                : __WASI_FILESTAT_SET_MTIM);
    }
  }

  __wasi_errno_t err =
      __wasi_fd_filestat_set_times(static_cast<__wasi_fd_t>(fd), atim, mtim,
                                   fstflags);
  if (err != __WASI_ERRNO_SUCCESS) {
    libc_errno = wasi_to_errno(err);
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
