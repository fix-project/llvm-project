//===-- WASI implementation of an exit function ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/__support/OSUtil/exit.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
namespace internal {

[[noreturn]] void exit(int status) {
  // WASI's proc_exit takes a u16 exit code.
  wasi::__wasi_proc_exit(static_cast<wasi::__wasi_errno_t>(status & 0xFFFF));
}

} // namespace internal
} // namespace LIBC_NAMESPACE_DECL
