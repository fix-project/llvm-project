//===-- WASI implementation of getpagesize --------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/getpagesize.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

// The minimum allocation unit of the WebAssembly linear memory.
#define WASI_PAGE_SIZE 65536

LLVM_LIBC_FUNCTION(int, getpagesize, (void)) { return WASI_PAGE_SIZE; }

} // namespace LIBC_NAMESPACE_DECL
