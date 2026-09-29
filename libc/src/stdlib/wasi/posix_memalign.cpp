//===-- WASI implementation of posix_memalign -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdlib/posix_memalign.h"

#include "src/__support/common.h"
#include "src/__support/freelist_heap.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include <stdint.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, posix_memalign,
                   (void **memptr, size_t alignment, size_t size)) {
  // The alignment must be a power of two and a multiple of sizeof(void *).
  if (alignment % sizeof(void *) != 0 || (alignment & (alignment - 1)) != 0)
    return EINVAL;

  void *ptr = freelist_heap->aligned_allocate(alignment, size);
  if (ptr == nullptr)
    return ENOMEM;

  *memptr = ptr;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
