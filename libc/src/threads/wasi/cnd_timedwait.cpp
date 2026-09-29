//===-- WASI implementation of the cnd_timedwait function ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/threads/cnd_timedwait.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"

#include <threads.h> // cnd_t, mtx_t, thrd_timedout

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, cnd_timedwait,
                   (cnd_t *__restrict cond, mtx_t *__restrict mtx,
                    const struct timespec *__restrict time_point)) {
  // WASI is single-threaded: a condition wait can never be satisfied, so
  // the wait always runs until the given timeout expires.
  (void)cond;
  (void)mtx;
  (void)time_point;
  return thrd_timedout;
}

} // namespace LIBC_NAMESPACE_DECL
