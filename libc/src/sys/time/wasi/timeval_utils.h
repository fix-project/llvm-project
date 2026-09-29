//===-- Helpers shared by the WASI time implementations -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SYS_TIME_WASI_TIMEVAL_UTILS_H
#define LLVM_LIBC_SRC_SYS_TIME_WASI_TIMEVAL_UTILS_H

#include "hdr/errno_macros.h"
#include "hdr/types/struct_timespec.h"
#include "hdr/types/struct_timeval.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

// Converts a two element timeval array to a timespec array as required by
// utimensat/futimens. Returns false and sets libc_errno on invalid input.
LIBC_INLINE bool timeval_to_timespec(const struct timeval times[2],
                                     struct timespec ts[2]) {
  for (int i = 0; i < 2; ++i) {
    if (times[i].tv_usec < 0 || times[i].tv_usec >= 1000000) {
      libc_errno = EINVAL;
      return false;
    }
    ts[i].tv_sec = times[i].tv_sec;
    ts[i].tv_nsec =
        static_cast<decltype(ts[i].tv_nsec)>(times[i].tv_usec * 1000);
  }
  return true;
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_SYS_TIME_WASI_TIMEVAL_UTILS_H
