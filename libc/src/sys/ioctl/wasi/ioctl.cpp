//===-- WASI implementation of ioctl --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/ioctl/ioctl.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

// WASI preview1 has no ioctl interface.
LLVM_LIBC_FUNCTION(int, ioctl, (int fd, unsigned long request, ...)) {
  (void)fd;
  (void)request;
  libc_errno = ENOTTY;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
