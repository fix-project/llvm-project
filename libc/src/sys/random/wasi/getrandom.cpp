//===-- WASI implementation of getrandom ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/random/getrandom.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, getrandom,
                   (void *buf, size_t buflen, unsigned int flags)) {
  (void)flags;
  if (buflen == 0)
    return 0;
  wasi::__wasi_errno_t err = wasi::__wasi_random_get(
      reinterpret_cast<uint8_t *>(buf),
      static_cast<wasi::__wasi_size_t>(buflen));
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  return static_cast<ssize_t>(buflen);
}

} // namespace LIBC_NAMESPACE_DECL
