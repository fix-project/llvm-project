//===-- Macros for <setjmp.h> --------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_SETJMP_MACROS_H
#define LLVM_LIBC_MACROS_SETJMP_MACROS_H

#if defined(__wasm__)

// On WebAssembly, setjmp/longjmp support is provided by the
// `-wasm-enable-sjlj` backend pass, which only recognizes calls to
// functions named `setjmp`/`longjmp`.  Map `sigsetjmp`/`siglongjmp` onto
// them so that the pass transforms the *caller's* frame (which is where
// the landing pad and function-invocation id must live).  WebAssembly has
// no asynchronous signal delivery, so the `savesigs` argument has no
// additional effect and is discarded.
#define sigsetjmp(buf, savesigs) setjmp(buf)
#define siglongjmp(buf, val) longjmp(buf, val)

#endif // __wasm__

#endif // LLVM_LIBC_MACROS_SETJMP_MACROS_H
