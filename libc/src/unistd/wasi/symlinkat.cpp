//===-- WASI implementation of symlinkat ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/symlinkat.h"

#include "symlink_utils.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, symlinkat,
                   (const char *target, int newdirfd, const char *linkpath)) {
  return wasi::symlinkat_impl(target, newdirfd, linkpath);
}

} // namespace LIBC_NAMESPACE_DECL
