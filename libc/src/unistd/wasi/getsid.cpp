//===-- WASI implementation of getsid -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/getsid.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/types/pid_t.h"

namespace LIBC_NAMESPACE_DECL {

// WASI reports a single fixed process id of 1.
LLVM_LIBC_FUNCTION(pid_t, getsid, (pid_t pid)) {
  if (pid != 0 && pid != 1) {
    libc_errno = ESRCH;
    return -1;
  }
  return 1;
}

} // namespace LIBC_NAMESPACE_DECL
