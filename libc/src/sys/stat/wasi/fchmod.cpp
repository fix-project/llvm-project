//===-- WASI implementation of fchmod -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/stat/fchmod.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/types/mode_t.h"

namespace LIBC_NAMESPACE_DECL {

// WASI preview1 has no permission model.
LLVM_LIBC_FUNCTION(int, fchmod, (int fd, mode_t mode)) {
  (void)fd;
  (void)mode;
  libc_errno = ENOSYS;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
