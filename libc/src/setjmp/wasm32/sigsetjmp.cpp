//===-- Implementation of sigsetjmp for WebAssembly -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// On WebAssembly, setjmp/longjmp support is provided by the
// `-wasm-enable-sjlj` backend pass, which only rewrites calls to functions
// named `setjmp`/`longjmp` and must transform the *caller's* frame (the
// landing pad and function-invocation id live in the frame that called
// setjmp).  A library-level wrapper therefore cannot work: the pass would
// degenerate the wrapper's own setjmp call, and the wrapper's frame would
// not survive the longjmp anyway.
//
// Instead, <setjmp.h> maps `sigsetjmp(buf, savesigs)` onto `setjmp(buf)`
// via a macro (see include/llvm-libc-macros/setjmp-macros.h), so the pass
// transforms the user's frame directly.  The exported symbol below only
// serves as a fallback for code that references it without including the
// header; reaching it at runtime is a bug, so trap loudly.
//
//===----------------------------------------------------------------------===//

#include "src/setjmp/sigsetjmp.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, sigsetjmp, (sigjmp_buf, int)) {
  // Unreachable in well-formed programs: the sigsetjmp macro in <setjmp.h>
  // redirects calls to setjmp, which the wasm-enable-sjlj pass lowers.
  __builtin_trap();
}

} // namespace LIBC_NAMESPACE_DECL
