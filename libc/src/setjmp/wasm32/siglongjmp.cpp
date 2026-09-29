//===-- Implementation of siglongjmp for WebAssembly ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// See src/setjmp/wasm32/sigsetjmp.cpp for an overview.  <setjmp.h> maps
// `siglongjmp(buf, val)` onto `longjmp(buf, val)` via a macro, so the pass
// lowers the call in the user's frame and this exported symbol is only a
// fallback for code that references it without including the header.
// WebAssembly has no asynchronous signal delivery, so there is no signal
// mask to restore.
//
//===----------------------------------------------------------------------===//

#include "src/setjmp/siglongjmp.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(void, siglongjmp, (jmp_buf, int)) {
  // Unreachable in well-formed programs: the siglongjmp macro in <setjmp.h>
  // redirects calls to longjmp, which the wasm-enable-sjlj pass lowers.
  __builtin_trap();
}

} // namespace LIBC_NAMESPACE_DECL
