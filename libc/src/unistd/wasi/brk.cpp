//===-- WASI implementation of brk ----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/brk.h"

#include "hdr/errno_macros.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/wasi_brk.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, brk, (void *address)) {
  if (wasi_set_break(reinterpret_cast<uintptr_t>(address)))
    return 0;
  libc_errno = ENOMEM;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
