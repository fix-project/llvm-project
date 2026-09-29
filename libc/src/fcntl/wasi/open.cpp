//===-- WASI implementation of open ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/fcntl/open.h"

#include "open_utils.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/types/mode_t.h"
#include <stdarg.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, open, (const char *path, int flags, ...)) {
  mode_t mode = 0;
  if (flags & O_CREAT) {
    va_list varargs;
    va_start(varargs, flags);
    mode = static_cast<mode_t>(va_arg(varargs, int));
    va_end(varargs);
  }

  int result = wasi::openat_impl(AT_FDCWD, path, flags, mode);
  if (result < 0) {
    libc_errno = -result;
    return -1;
  }
  return result;
}

} // namespace LIBC_NAMESPACE_DECL
