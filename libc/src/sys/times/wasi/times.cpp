//===-- WASI implementation of times -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/times/times.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(clock_t, times, (struct tms * buf)) {
  if (buf == nullptr) {
    libc_errno = EFAULT;
    return static_cast<clock_t>(-1);
  }

  wasi::__wasi_timestamp_t elapsed, cpu = 0;
  wasi::__wasi_errno_t err =
      wasi::__wasi_clock_time_get(wasi::__WASI_CLOCKID_MONOTONIC, 1, &elapsed);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return static_cast<clock_t>(-1);
  }

  // The process CPU clock is optional in WASIp1 runtimes. An unavailable
  // clock contributes no accounting instead of making elapsed time unusable.
  wasi::__wasi_clock_time_get(wasi::__WASI_CLOCKID_PROCESS_CPUTIME_ID, 1, &cpu);

  // Avoid overflowing the 32-bit WASI clock_t after only a few seconds.
  constexpr wasi::__wasi_timestamp_t NS_PER_TICK = 10000000;
  static wasi::__wasi_timestamp_t start_time = elapsed;
  buf->tms_utime = static_cast<clock_t>(cpu / NS_PER_TICK);
  buf->tms_stime = 0;
  buf->tms_cutime = 0;
  buf->tms_cstime = 0;
  return static_cast<clock_t>((elapsed - start_time) / NS_PER_TICK);
}

} // namespace LIBC_NAMESPACE_DECL
