//===-- Implementation of vdprintf ----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdio/vdprintf.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/stdio/vasprintf.h"
#include "src/stdlib/free.h"
#include "src/unistd/write.h"

#include <errno.h>
#include <stddef.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, vdprintf,
                   (int fd, const char *__restrict format, va_list args)) {
  char *buffer = nullptr;
  int length = LIBC_NAMESPACE::vasprintf(&buffer, format, args);
  if (length < 0)
    return -1;

  if (length == 0) {
    ssize_t count = LIBC_NAMESPACE::write(fd, buffer, 0);
    LIBC_NAMESPACE::free(buffer);
    return count < 0 ? -1 : 0;
  }

  size_t written = 0;
  while (written < static_cast<size_t>(length)) {
    ssize_t count = LIBC_NAMESPACE::write(
        fd, buffer + written, static_cast<size_t>(length) - written);
    if (count < 0) {
      if (libc_errno == EINTR)
        continue;
      LIBC_NAMESPACE::free(buffer);
      return -1;
    }
    if (count == 0) {
      libc_errno = EIO;
      LIBC_NAMESPACE::free(buffer);
      return -1;
    }
    written += static_cast<size_t>(count);
  }
  LIBC_NAMESPACE::free(buffer);
  return length;
}

} // namespace LIBC_NAMESPACE_DECL
