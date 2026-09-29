//===-- WASI umask emulation ----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/stat/umask.h"

#include "src/__support/common.h"

namespace LIBC_NAMESPACE_DECL {

// WASI preview1 does not expose permission bits. Preserve the process mask
// so programs that inspect or temporarily change it still behave consistently.
static mode_t process_mask = 0022;

LLVM_LIBC_FUNCTION(mode_t, umask, (mode_t mask)) {
  mode_t old = process_mask;
  process_mask = mask & 0777;
  return old;
}

} // namespace LIBC_NAMESPACE_DECL
