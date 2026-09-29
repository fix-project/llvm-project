//===-- WASI implementation of setrlimit ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/resource/setrlimit.h"
#include "src/sys/resource/getrlimit.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/sys_resource_macros.h"

namespace LIBC_NAMESPACE_DECL {

// WASI cannot enforce resource limits. Accept only the current values, so a
// successful call never promises a limit that the runtime does not enforce.
LLVM_LIBC_FUNCTION(int, setrlimit, (int resource, const struct rlimit *lim)) {
  if (lim == nullptr) {
    libc_errno = EFAULT;
    return -1;
  }
  if (lim->rlim_cur > lim->rlim_max) {
    libc_errno = EINVAL;
    return -1;
  }
  struct rlimit current;
  if (LIBC_NAMESPACE::getrlimit(resource, &current) != 0)
    return -1;
  if (lim->rlim_cur == current.rlim_cur && lim->rlim_max == current.rlim_max)
    return 0;
  libc_errno = ENOTSUP;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
