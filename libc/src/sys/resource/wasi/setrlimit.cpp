//===-- WASI implementation of setrlimit ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/resource/setrlimit.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/sys_resource_macros.h"

namespace LIBC_NAMESPACE_DECL {

// WASI has no resource limits; accept any request that does not lower the
// (infinite) limits below the fixed values reported by getrlimit().
LLVM_LIBC_FUNCTION(int, setrlimit,
                   (int resource, const struct rlimit *lim)) {
  if (lim == nullptr) {
    libc_errno = EFAULT;
    return -1;
  }
  if (lim->rlim_cur > lim->rlim_max) {
    libc_errno = EINVAL;
    return -1;
  }
  (void)resource;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
