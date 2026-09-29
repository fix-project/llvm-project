//===-- WASI implementation of the cnd_wait function ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/threads/cnd_wait.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"

#include <threads.h> // cnd_t, mtx_t, thrd_error

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, cnd_wait, (cnd_t * cond, mtx_t *mtx)) {
  // WASI is single-threaded: a condition wait can never be satisfied.
  (void)cond;
  (void)mtx;
  return thrd_error;
}

} // namespace LIBC_NAMESPACE_DECL
