//===-- WASI implementation of the mtx_init function ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/threads/mtx_init.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/__support/macros/null_check.h"

#include <threads.h> // mtx_t, thrd_error, thrd_success

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, mtx_init, (mtx_t * mutex, int type)) {
  LIBC_CRASH_ON_NULLPTR(mutex);
  mutex->__locked = 0;
  mutex->__priority_inherit = 0;
  mutex->__recursive = (type & mtx_recursive) ? 1U : 0U;
  mutex->__robust = 0;
  mutex->__pshared = 0;
  mutex->__error_checking = 0;
  mutex->__owner = 0;
  mutex->__lock_count = 0;
  return thrd_success;
}

} // namespace LIBC_NAMESPACE_DECL
