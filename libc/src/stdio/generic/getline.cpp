//===-- Portable implementation of getline -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdio/getline.h"

#include "src/__support/common.h"
#include "src/stdio/getdelim.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, getline,
                   (char **lineptr, size_t *n, ::FILE *stream)) {
  return getdelim(lineptr, n, '\n', stream);
}

} // namespace LIBC_NAMESPACE_DECL
