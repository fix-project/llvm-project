//===-- WASI implementation of link ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/link.h"

#include "link_utils.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, link, (const char *oldpath, const char *newpath)) {
  return wasi::linkat_impl(AT_FDCWD, oldpath, AT_FDCWD, newpath,
                           AT_SYMLINK_FOLLOW);
}

} // namespace LIBC_NAMESPACE_DECL
