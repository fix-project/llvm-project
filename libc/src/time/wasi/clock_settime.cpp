//===-- WASI implementation of clock_settime ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// WASI provides no way for a sandboxed process to set system clocks, so
// every clock is treated as non-settable.  Arguments are validated the
// same way as on POSIX systems and CLOCK_REALTIME fails with EPERM while
// the remaining clocks fail with EINVAL, matching the behavior expected
// for unprivileged processes.
//
//===----------------------------------------------------------------------===//

#include "src/time/clock_settime.h"
#include "hdr/time_macros.h"
#include "hdr/types/clockid_t.h"
#include "hdr/types/struct_timespec.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, clock_settime,
                   (clockid_t clockid, const timespec *ts)) {
  if (ts == nullptr) {
    libc_errno = EFAULT;
    return -1;
  }

  bool realtime = false;
  switch (clockid) {
  case CLOCK_REALTIME:
    realtime = true;
    break;
  case CLOCK_MONOTONIC:
  case CLOCK_PROCESS_CPUTIME_ID:
  case CLOCK_THREAD_CPUTIME_ID:
    break;
  default:
    libc_errno = EINVAL;
    return -1;
  }

  if (ts->tv_nsec < 0 || ts->tv_nsec >= 1000000000L) {
    libc_errno = EINVAL;
    return -1;
  }

  libc_errno = realtime ? EPERM : EINVAL;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
