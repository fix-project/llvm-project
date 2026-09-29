//===-- WASI implementation of alarm --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/alarm.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

// WASI has no interval timer facility; no alarm can be pending.
LLVM_LIBC_FUNCTION(unsigned int, alarm, (unsigned int seconds)) {
  (void)seconds;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
