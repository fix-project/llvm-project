//===-- WASI implementation of readlink -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/readlink.h"

#include "readlink_utils.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(ssize_t, readlink,
                   (const char *__restrict path, char *__restrict buf,
                    size_t bufsize)) {
  return wasi::readlink_impl(AT_FDCWD, path, buf, bufsize);
}

} // namespace LIBC_NAMESPACE_DECL
