//===-- WASI implementation of nanosleep ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/time/nanosleep.h"
#include "hdr/types/struct_timespec.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, nanosleep, (const timespec *req, timespec *rem)) {
  constexpr uint64_t NS_PER_SEC = 1000000000ULL;

  if (req == nullptr) {
    libc_errno = EFAULT;
    return -1;
  }
  if (req->tv_sec < 0 || req->tv_nsec < 0 || req->tv_nsec >= 1000000000) {
    libc_errno = EINVAL;
    return -1;
  }

  if (rem != nullptr) {
    rem->tv_sec = 0;
    rem->tv_nsec = 0;
  }

  uint64_t seconds = static_cast<uint64_t>(req->tv_sec);
  uint64_t timeout = seconds * NS_PER_SEC + static_cast<uint64_t>(req->tv_nsec);
  if (timeout == 0)
    timeout = 1;

  // WASI has no signal delivery, so the sleep always runs to completion.
  wasi::__wasi_subscription_t sub = {};
  sub.u.tag = wasi::__WASI_EVENTTYPE_CLOCK;
  sub.u.u.clock.id = wasi::__WASI_CLOCKID_MONOTONIC;
  sub.u.u.clock.flags = 0; // Relative timeout.
  sub.u.u.clock.timeout = timeout;
  sub.u.u.clock.precision = 1;

  wasi::__wasi_event_t events[1];
  wasi::__wasi_size_t nevents = 0;
  wasi::__wasi_errno_t err =
      wasi::__wasi_poll_oneoff(&sub, events, 1, &nevents);
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = static_cast<int>(err);
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
