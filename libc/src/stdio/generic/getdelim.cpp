//===-- Portable implementation of getdelim ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdio/getdelim.h"

#include "hdr/stdio_macros.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/stdio/ferror.h"
#include "src/stdio/fgetc.h"
#include "src/stdlib/realloc.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, getdelim,
                   (char **lineptr, size_t *n, int delimiter, ::FILE *stream)) {
  if (lineptr == nullptr || n == nullptr || stream == nullptr) {
    libc_errno = EINVAL;
    return -1;
  }
  if (*lineptr == nullptr)
    *n = 0;
  size_t used = 0;
  for (;;) {
    int ch = fgetc(stream);
    if (ch == EOF) {
      if (ferror(stream) || used == 0)
        return -1;
      break;
    }
    if (used >= static_cast<size_t>(__PTRDIFF_MAX__)) {
      libc_errno = EOVERFLOW;
      return -1;
    }
    if (used + 2 > *n) {
      size_t capacity = *n < 128 ? 128 : *n;
      while (capacity < used + 2) {
        if (capacity > static_cast<size_t>(-1) / 2) {
          libc_errno = ENOMEM;
          return -1;
        }
        capacity *= 2;
      }
      char *grown = static_cast<char *>(realloc(*lineptr, capacity));
      if (grown == nullptr)
        return -1;
      *lineptr = grown;
      *n = capacity;
    }
    (*lineptr)[used++] = static_cast<char>(ch);
    if (ch == static_cast<unsigned char>(delimiter))
      break;
  }
  (*lineptr)[used] = '\0';
  return static_cast<ssize_t>(used);
}

} // namespace LIBC_NAMESPACE_DECL
