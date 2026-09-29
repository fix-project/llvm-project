//===-- WASI implementation of wait4 --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/wait/wait4.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(pid_t, wait4,
                   (pid_t pid, int *wait_status, int options,
                    struct rusage *usage)) {
  (void)pid;
  (void)wait_status;
  (void)options;
  (void)usage;
  // WASI has no child processes.
  libc_errno = ECHILD;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
