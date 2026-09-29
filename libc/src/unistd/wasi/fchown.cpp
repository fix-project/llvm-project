//===-- WASI implementation of fchown -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/fchown.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/types/gid_t.h"
#include "hdr/types/uid_t.h"

namespace LIBC_NAMESPACE_DECL {

// WASI preview1 has no ownership model.
LLVM_LIBC_FUNCTION(int, fchown, (int fd, uid_t owner, gid_t group)) {
  (void)fd;
  (void)owner;
  (void)group;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
