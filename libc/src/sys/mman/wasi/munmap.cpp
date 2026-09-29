//===-- WASI mmap emulation: munmap -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for more information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/mman/munmap.h"
#include "src/sys/mman/wasi/mman_emulation.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"

#include <stdlib.h>

namespace LIBC_NAMESPACE_DECL {
namespace mman_wasi {

LLVM_LIBC_FUNCTION(int, munmap, (void *addr, size_t size)) {
  lock_mman();
  Mapping *m = find_mapping(addr);
  if (m == nullptr) {
    unlock_mman();
    libc_errno = EINVAL;
    return -1;
  }
  if (size != m->size) {
    unlock_mman();
    libc_errno = EINVAL;
    return -1;
  }
  bool ok = flush_mapping(*m);
  free(m->addr);
  release_mapping(m);
  unlock_mman();
  if (!ok) {
    libc_errno = EIO;
    return -1;
  }
  return 0;
}

} // namespace mman_wasi
} // namespace LIBC_NAMESPACE_DECL
