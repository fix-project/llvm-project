//===-- WASI implementation of timespec_get -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/time/timespec_get.h"
#include "hdr/time_macros.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "src/__support/time/clock_gettime.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, timespec_get, (timespec * ts, int base)) {
  clockid_t clockid;
  switch (base) {
  case TIME_UTC:
    clockid = CLOCK_REALTIME;
    break;
#ifdef TIME_MONOTONIC
  case TIME_MONOTONIC:
    clockid = CLOCK_MONOTONIC;
    break;
#endif
#ifdef TIME_ACTIVE
  case TIME_ACTIVE:
    clockid = CLOCK_PROCESS_CPUTIME_ID;
    break;
#endif
#ifdef TIME_THREAD_ACTIVE
  case TIME_THREAD_ACTIVE:
    clockid = CLOCK_THREAD_CPUTIME_ID;
    break;
#endif
  default:
    return 0;
  }

  auto result = internal::clock_gettime(clockid, ts);
  if (!result.has_value()) {
    libc_errno = result.error();
    return 0;
  }
  return base;
}

} // namespace LIBC_NAMESPACE_DECL
