//===-- WASI confstr -------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/confstr.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "hdr/unistd_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(size_t, confstr, (int name, char *buffer, size_t length)) {
  (void)buffer;
  (void)length;
  if (name == _CS_PATH)
    return 0; // WASIp1 has no process execution search path.
  libc_errno = EINVAL;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
