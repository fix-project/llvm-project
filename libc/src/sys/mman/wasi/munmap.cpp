//===-- WASI mmap emulation: munmap -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for more information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/mman/munmap.h"
#include "src/sys/mman/wasi/mman_emulation.h"

#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"

#include <stdlib.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, munmap, (void *addr, size_t size)) {
  mman_wasi::lock_mman();
  mman_wasi::Mapping *m = mman_wasi::find_mapping(addr);
  if (m == nullptr) {
    mman_wasi::unlock_mman();
    libc_errno = EINVAL;
    return -1;
  }
  if (size != m->size) {
    mman_wasi::unlock_mman();
    libc_errno = EINVAL;
    return -1;
  }
  bool ok = mman_wasi::flush_mapping(*m);
  if (m->fd >= 0) {
    wasi::unregister_fd_path(m->fd);
    wasi::unmark_fd_o_path(m->fd);
    wasi::__wasi_fd_close(static_cast<wasi::__wasi_fd_t>(m->fd));
  }
  free(m->addr);
  mman_wasi::release_mapping(m);
  mman_wasi::unlock_mman();
  if (!ok) {
    libc_errno = EIO;
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
