//===-- WASI implementation of sbrk ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/sbrk.h"

#include "hdr/errno_macros.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/wasi_brk.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(void *, sbrk, (intptr_t increment)) {
  uintptr_t previous = wasi_current_break();
  uintptr_t address;
  if (increment >= 0) {
    if (__builtin_add_overflow(previous, static_cast<uintptr_t>(increment),
                               &address)) {
      libc_errno = ENOMEM;
      return reinterpret_cast<void *>(-1);
    }
  } else {
    // Negating INTPTR_MIN directly would overflow.
    uintptr_t decrement = static_cast<uintptr_t>(-(increment + 1)) + 1;
    if (decrement > previous) {
      libc_errno = ENOMEM;
      return reinterpret_cast<void *>(-1);
    }
    address = previous - decrement;
  }
  if (!wasi_set_break(address)) {
    libc_errno = ENOMEM;
    return reinterpret_cast<void *>(-1);
  }
  return reinterpret_cast<void *>(previous);
}

} // namespace LIBC_NAMESPACE_DECL
