//===-- WASI implementation of getrusage ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/resource/getrusage.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/sys_resource_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, getrusage, (int who, struct rusage *usage)) {
  if (usage == nullptr) {
    libc_errno = EFAULT;
    return -1;
  }
  if (who != RUSAGE_SELF && who != RUSAGE_CHILDREN) {
    libc_errno = EINVAL;
    return -1;
  }

  *usage = {};
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
