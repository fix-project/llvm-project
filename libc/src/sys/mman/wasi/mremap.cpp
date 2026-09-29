//===-- WASI mmap emulation: mremap ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/mman/mremap.h"
#include "src/sys/mman/wasi/mman_emulation.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(void *, mremap,
                   (void *old_address, size_t old_size, size_t new_size,
                    int flags, ...)) {
  if (new_size == 0 || (flags & ~MREMAP_MAYMOVE) != 0) {
    libc_errno = EINVAL;
    return MAP_FAILED;
  }

  mman_wasi::lock_mman();
  mman_wasi::Mapping *m = mman_wasi::find_mapping(old_address);
  if (m == nullptr || m->size != old_size) {
    mman_wasi::unlock_mman();
    libc_errno = EINVAL;
    return MAP_FAILED;
  }
  if (new_size == old_size) {
    mman_wasi::unlock_mman();
    return old_address;
  }
  if (new_size < old_size) {
    m->size = new_size;
    mman_wasi::unlock_mman();
    return old_address;
  }
  if ((flags & MREMAP_MAYMOVE) == 0) {
    mman_wasi::unlock_mman();
    libc_errno = ENOMEM;
    return MAP_FAILED;
  }
  if (m->file_backed && m->fd < 0) {
    mman_wasi::unlock_mman();
    libc_errno = ENOTSUP;
    return MAP_FAILED;
  }

  constexpr size_t PAGE_SIZE = 4096;
  if (new_size > SIZE_MAX - (PAGE_SIZE - 1)) {
    mman_wasi::unlock_mman();
    libc_errno = ENOMEM;
    return MAP_FAILED;
  }
  size_t rounded = (new_size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
  void *new_address = aligned_alloc(PAGE_SIZE, rounded);
  if (new_address == nullptr) {
    mman_wasi::unlock_mman();
    libc_errno = ENOMEM;
    return MAP_FAILED;
  }
  memcpy(new_address, m->addr, old_size);
  memset(static_cast<char *>(new_address) + old_size, 0, new_size - old_size);
  free(m->addr);
  m->addr = new_address;
  m->size = new_size;
  mman_wasi::unlock_mman();
  return new_address;
}

} // namespace LIBC_NAMESPACE_DECL
