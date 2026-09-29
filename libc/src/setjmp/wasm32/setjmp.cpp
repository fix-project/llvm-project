//===-- Implementation of setjmp for WebAssembly --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// WebAssembly has no machine stack pointer to save/restore, so setjmp/longjmp
// cannot be implemented the way native targets do. Instead they rely on the
// LLVM backend: when the target is built with the wasm-enable-sjlj backend pass
// (which requires the exception-handling, multivalue and reference-types
// features) the WebAssemblyLowerEmscriptenEHSjLj pass rewrites every *call* to
// setjmp/longjmp into calls to the __wasm_setjmp / __wasm_setjmp_test /
// __wasm_longjmp runtime helpers defined here. The helpers communicate the
// unwound control flow using the exception-handling `throw` instruction.
//
//===----------------------------------------------------------------------===//

#include "src/setjmp/setjmp_impl.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/__support/macros/properties/cpu_features.h"
#include "src/setjmp/wasm32/wasm_sjlj.h"

#include <stdint.h>

namespace LIBC_NAMESPACE_DECL {

#if defined(LIBC_TARGET_CPU_HAS_WASM_SJLJ)

// The real setjmp is produced by the backend pass; a direct call reaching this
// definition means the pass did not run, which is a build misconfiguration.
LLVM_LIBC_FUNCTION(int, setjmp, (jmp_buf)) { __builtin_trap(); }

} // namespace LIBC_NAMESPACE_DECL

// Runtime support called by the compiler-generated setjmp lowering. These must
// have external C linkage with these exact names; see
// llvm/lib/Target/WebAssembly/WebAssemblyLowerEmscriptenEHSjLj.cpp.
extern "C" {

void __wasm_setjmp(void *env, uint32_t label, void *func_invocation_id) {
  LIBC_NAMESPACE::wasm_sjlj::jmp_buf_impl *jb =
      static_cast<LIBC_NAMESPACE::wasm_sjlj::jmp_buf_impl *>(env);
  // label == 0 is reserved by the ABI; a null invocation id is a sanity check.
  if (label == 0 || func_invocation_id == nullptr)
    __builtin_trap();
  jb->func_invocation_id = func_invocation_id;
  jb->label = label;
}

uint32_t __wasm_setjmp_test(void *env, void *func_invocation_id) {
  LIBC_NAMESPACE::wasm_sjlj::jmp_buf_impl *jb =
      static_cast<LIBC_NAMESPACE::wasm_sjlj::jmp_buf_impl *>(env);
  if (jb->label == 0 || func_invocation_id == nullptr)
    __builtin_trap();
  if (jb->func_invocation_id == func_invocation_id)
    return jb->label;
  return 0;
}

} // extern "C"

#else // !LIBC_TARGET_CPU_HAS_WASM_SJLJ

LLVM_LIBC_FUNCTION(int, setjmp, (jmp_buf)) {
  // setjmp/longjmp require the WebAssembly exception-handling based sjlj
  // lowering; build with the wasm-enable-sjlj backend pass to enable them.
  __builtin_trap();
}

} // namespace LIBC_NAMESPACE_DECL

#endif // LIBC_TARGET_CPU_HAS_WASM_SJLJ
