//===-- WASI implementation of readlinkat ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/readlinkat.h"

#include "readlink_utils.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, readlinkat,
                   (int dirfd, const char *__restrict path,
                    char *__restrict buf, size_t bufsize)) {
  return wasi::readlink_impl(dirfd, path, buf, bufsize);
}

} // namespace LIBC_NAMESPACE_DECL
