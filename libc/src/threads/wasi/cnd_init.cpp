//===-- WASI implementation of the cnd_init function ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/threads/cnd_init.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include <threads.h> // cnd_t, thrd_error, thrd_success

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, cnd_init, (cnd_t * cond)) {
  if (cond == nullptr) {
    libc_errno = EINVAL;
    return thrd_error;
  }
  *cond = {};
  return thrd_success;
}

} // namespace LIBC_NAMESPACE_DECL
