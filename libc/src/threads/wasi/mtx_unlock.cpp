//===-- WASI implementation of the mtx_unlock function -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/threads/mtx_unlock.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/__support/macros/null_check.h"

#include <threads.h> // mtx_t, thrd_error, thrd_success

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, mtx_unlock, (mtx_t * mutex)) {
  LIBC_CRASH_ON_NULLPTR(mutex);
  if (!mutex->__locked)
    return thrd_error;
  if (mutex->__recursive && mutex->__lock_count > 1) {
    --mutex->__lock_count;
    return thrd_success;
  }
  mutex->__locked = 0;
  mutex->__lock_count = 0;
  return thrd_success;
}

} // namespace LIBC_NAMESPACE_DECL
