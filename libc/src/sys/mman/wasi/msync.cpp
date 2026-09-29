//===-- WASI mmap emulation: msync --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for more information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/mman/msync.h"
#include "src/sys/mman/wasi/mman_emulation.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include <stdint.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, msync, (void *addr, size_t size, int flags)) {
  (void)flags;
  mman_wasi::lock_mman();
  mman_wasi::Mapping *m = nullptr;
  size_t begin = 0;
  uintptr_t requested = reinterpret_cast<uintptr_t>(addr);
  for (unsigned i = 0; i < mman_wasi::mapping_capacity(); ++i) {
    mman_wasi::Mapping *candidate = &mman_wasi::mapping_table()[i];
    if (candidate->addr == nullptr)
      continue;
    uintptr_t base = reinterpret_cast<uintptr_t>(candidate->addr);
    if (requested >= base && requested - base < candidate->size) {
      m = candidate;
      begin = requested - base;
      break;
    }
  }
  if (m == nullptr) {
    mman_wasi::unlock_mman();
    libc_errno = ENOMEM;
    return -1;
  }
  if (size == 0 || size > m->size - begin) {
    mman_wasi::unlock_mman();
    libc_errno = EINVAL;
    return -1;
  }
  bool ok = mman_wasi::flush_mapping_range(*m, begin, size);
  mman_wasi::unlock_mman();
  if (!ok) {
    libc_errno = EIO;
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
