//===-- WASI implementation of the cnd_signal function -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/threads/cnd_signal.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"

#include <threads.h> // cnd_t, thrd_success

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, cnd_signal, (cnd_t * cond)) {
  // WASI is single-threaded: there can never be a waiting thread.
  (void)cond;
  return thrd_success;
}

} // namespace LIBC_NAMESPACE_DECL
