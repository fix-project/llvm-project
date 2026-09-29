//===-- WASI implementation of lutimes ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/time/lutimes.h"

#include "hdr/fcntl_macros.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/sys/stat/utimensat.h"
#include "src/sys/time/wasi/timeval_utils.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, lutimes,
                   (const char *path, const struct timeval times[2])) {
  struct timespec ts[2];
  const struct timespec *ts_ptr = nullptr;
  if (times != nullptr) {
    if (!wasi::timeval_to_timespec(times, ts))
      return -1;
    ts_ptr = ts;
  }
  return utimensat(AT_FDCWD, path, ts_ptr, AT_SYMLINK_NOFOLLOW);
}

} // namespace LIBC_NAMESPACE_DECL
