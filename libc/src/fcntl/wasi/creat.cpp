//===-- WASI implementation of creat --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/fcntl/creat.h"

#include "open_utils.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/fcntl_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, creat, (const char *path, int mode)) {
  int result = wasi::openat_impl(AT_FDCWD, path, O_WRONLY | O_CREAT | O_TRUNC,
                                 mode);
  if (result < 0) {
    libc_errno = -result;
    return -1;
  }
  return result;
}

} // namespace LIBC_NAMESPACE_DECL
