//===-- WASI implementation of getrlimit ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/resource/getrlimit.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/sys_resource_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, getrlimit, (int resource, struct rlimit *lim)) {
  if (lim == nullptr) {
    libc_errno = EFAULT;
    return -1;
  }
  switch (resource) {
  case RLIMIT_NOFILE:
    lim->rlim_cur = 1024;
    lim->rlim_max = 1024;
    return 0;
  case RLIMIT_STACK:
    lim->rlim_cur = 8388608;
    lim->rlim_max = RLIM_INFINITY;
    return 0;
  case RLIMIT_CPU:
  case RLIMIT_FSIZE:
  case RLIMIT_DATA:
  case RLIMIT_CORE:
  case RLIMIT_RSS:
  case RLIMIT_NPROC:
  case RLIMIT_MEMLOCK:
  case RLIMIT_AS:
    lim->rlim_cur = RLIM_INFINITY;
    lim->rlim_max = RLIM_INFINITY;
    return 0;
  default:
    libc_errno = EINVAL;
    return -1;
  }
}

} // namespace LIBC_NAMESPACE_DECL
