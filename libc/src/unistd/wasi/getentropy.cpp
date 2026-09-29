//===-- WASI implementation of getentropy ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/getentropy.h"

#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, getentropy, (void *buffer, size_t length)) {
  // POSIX requires EIO for requests larger than 256 bytes; a null buffer
  // also reports EIO to match the platform behaviour the tests expect.
  if (length > 256 || buffer == nullptr) {
    libc_errno = EIO;
    return -1;
  }
  wasi::__wasi_errno_t err = wasi::__wasi_random_get(
      static_cast<unsigned char *>(buffer),
      static_cast<wasi::__wasi_size_t>(length));
  if (err != wasi::__WASI_ERRNO_SUCCESS) {
    libc_errno = wasi::wasi_to_errno(err);
    return -1;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
