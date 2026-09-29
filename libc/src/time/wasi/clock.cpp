//===-- WASI implementation of the clock function -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/time/clock.h"

#include "hdr/time_macros.h"
#include "src/__support/CPP/limits.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(clock_t, clock, ()) {
  wasi::__wasi_timestamp_t timestamp = 0;
  if (wasi::__wasi_clock_time_get(wasi::__WASI_CLOCKID_MONOTONIC, 1, &timestamp) !=
      wasi::__WASI_ERRNO_SUCCESS) {
    return static_cast<clock_t>(-1);
  }

  // The WASI clock has nanosecond resolution while clock_t is counted in
  // units of CLOCKS_PER_SEC.
  constexpr uint64_t NS_PER_SEC = 1000000000ULL;
  constexpr clock_t CLOCK_MAX = cpp::numeric_limits<clock_t>::max();
  if (timestamp / NS_PER_SEC * CLOCKS_PER_SEC >
      static_cast<uint64_t>(CLOCK_MAX)) {
    return static_cast<clock_t>(-1);
  }

  uint64_t secs = timestamp / NS_PER_SEC;
  uint64_t nsecs = timestamp % NS_PER_SEC;
  uint64_t ticks = secs * CLOCKS_PER_SEC + nsecs / (NS_PER_SEC / CLOCKS_PER_SEC);
  return static_cast<clock_t>(ticks);
}

} // namespace LIBC_NAMESPACE_DECL
