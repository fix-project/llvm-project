//===-- WASI getlogin_r ----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/getlogin_r.h"
#include "src/__support/common.h"
#include "hdr/errno_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, getlogin_r, (char *name, size_t namesize)) {
  (void)name;
  (void)namesize;
  return ENXIO;
}

} // namespace LIBC_NAMESPACE_DECL
