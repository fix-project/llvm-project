//===-- WASI implementation of gethostname --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/gethostname.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, gethostname, (char * name, size_t len)) {
  static constexpr char HOSTNAME[] = "wasi";
  constexpr size_t HOSTNAME_LEN = sizeof(HOSTNAME) - 1;

  if (name == nullptr) {
    libc_errno = EFAULT;
    return -1;
  }
  if (len == 0) {
    libc_errno = ENAMETOOLONG;
    return -1;
  }

  size_t i = 0;
  for (; i < HOSTNAME_LEN && i + 1 < len; ++i)
    name[i] = HOSTNAME[i];
  name[i] = '\0';

  if (i < HOSTNAME_LEN) {
    libc_errno = ENAMETOOLONG;
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
