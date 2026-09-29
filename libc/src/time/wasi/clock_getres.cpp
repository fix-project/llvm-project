//===-- WASI implementation of clock_getres -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/time/clock_getres.h"
#include "hdr/time_macros.h"
#include "hdr/types/clockid_t.h"
#include "hdr/types/struct_timespec.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, clock_getres, (clockid_t clockid, timespec *tp)) {
  constexpr uint64_t NS_PER_SEC = 1000000000ULL;

  wasi::__wasi_clockid_t wid;
  switch (clockid) {
  case CLOCK_REALTIME:
    wid = wasi::__WASI_CLOCKID_REALTIME;
    break;
  case CLOCK_MONOTONIC:
    wid = wasi::__WASI_CLOCKID_MONOTONIC;
    break;
  case CLOCK_PROCESS_CPUTIME_ID:
    wid = wasi::__WASI_CLOCKID_PROCESS_CPUTIME_ID;
    break;
  case CLOCK_THREAD_CPUTIME_ID:
    wid = wasi::__WASI_CLOCKID_THREAD_CPUTIME_ID;
    break;
  default:
    libc_errno = EINVAL;
    return -1;
  }

  wasi::__wasi_timestamp_t resolution = 0;
  wasi::__wasi_errno_t err = wasi::__wasi_clock_res_get(wid, &resolution);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = static_cast<int>(err);
    return -1;
  }

  if (tp != nullptr) {
    tp->tv_sec = static_cast<decltype(tp->tv_sec)>(resolution / NS_PER_SEC);
    tp->tv_nsec =
        static_cast<decltype(tp->tv_nsec)>(resolution % NS_PER_SEC);
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
