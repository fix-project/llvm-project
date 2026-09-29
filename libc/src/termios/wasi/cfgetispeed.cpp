//===-- WASI implementation of cfgetispeed -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/termios/cfgetispeed.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(speed_t, cfgetispeed, (const struct termios *t)) {
  (void)t;
  // WASI has no terminal interface.
  libc_errno = ENOSYS;
  return static_cast<speed_t>(-1);
}

} // namespace LIBC_NAMESPACE_DECL
