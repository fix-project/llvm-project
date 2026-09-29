//===-- WASI getlogin ------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/getlogin.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(char *, getlogin, (void)) {
  // A WASIp1 instance has neither a login session nor a controlling terminal.
  libc_errno = ENXIO;
  return nullptr;
}

} // namespace LIBC_NAMESPACE_DECL
