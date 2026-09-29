//===-- WASI mmap emulation: mprotect -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for more information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/mman/mprotect.h"

#include "src/__support/common.h"

namespace LIBC_NAMESPACE_DECL {

// The mmap emulation serves memory from the regular heap, so page protection
// cannot be changed. Accept the request as a no-op so that callers which only
// tighten or relax permissions keep working.
LLVM_LIBC_FUNCTION(int, mprotect, (void *addr, size_t size, int prot)) {
  (void)addr;
  (void)size;
  (void)prot;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
