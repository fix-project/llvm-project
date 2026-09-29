//===-- WASI implementation of internal::clock_gettime --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/__support/time/clock_gettime.h"
#include "hdr/time_macros.h"
#include "hdr/types/clockid_t.h"
#include "hdr/types/struct_timespec.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/error_or.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
namespace internal {

// The POSIX clock ids exposed by WASI-enabled toolchains
// (CLOCK_REALTIME=0, CLOCK_MONOTONIC=1, CLOCK_PROCESS_CPUTIME_ID=2,
// CLOCK_THREAD_CPUTIME_ID=3) coincide with the WASI clock ids, but map them
// explicitly so that unsupported ids are rejected.
static bool to_wasi_clockid(clockid_t clockid, wasi::__wasi_clockid_t *out) {
  switch (clockid) {
  case CLOCK_REALTIME:
    *out = wasi::__WASI_CLOCKID_REALTIME;
    return true;
  case CLOCK_MONOTONIC:
    *out = wasi::__WASI_CLOCKID_MONOTONIC;
    return true;
  case CLOCK_PROCESS_CPUTIME_ID:
    *out = wasi::__WASI_CLOCKID_PROCESS_CPUTIME_ID;
    return true;
  case CLOCK_THREAD_CPUTIME_ID:
    *out = wasi::__WASI_CLOCKID_THREAD_CPUTIME_ID;
    return true;
  default:
    return false;
  }
}

ErrorOr<int> clock_gettime(clockid_t clockid, timespec *ts) {
  constexpr uint64_t NS_PER_SEC = 1000000000ULL;

  wasi::__wasi_clockid_t wid;
  if (!to_wasi_clockid(clockid, &wid))
    return EINVAL;

  wasi::__wasi_timestamp_t timestamp = 0;
  wasi::__wasi_errno_t err = wasi::__wasi_clock_time_get(wid, 1, &timestamp);
  if (err != wasi::__WASI_ERRNO_SUCCESS)
    return static_cast<int>(err);

  if (ts != nullptr) {
    ts->tv_sec = static_cast<decltype(ts->tv_sec)>(timestamp / NS_PER_SEC);
    ts->tv_nsec =
        static_cast<decltype(ts->tv_nsec)>(timestamp % NS_PER_SEC);
  }
  return 0;
}

} // namespace internal
} // namespace LIBC_NAMESPACE_DECL
