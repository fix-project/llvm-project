//===-- Implementation of longjmp for WebAssembly -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// See src/setjmp/wasm32/setjmp.cpp for an overview of how setjmp/longjmp work
// on WebAssembly. longjmp is lowered by the backend to a call to
// __wasm_longjmp, which unwinds using the exception-handling `throw`
// instruction.
//
//===----------------------------------------------------------------------===//

#include "src/setjmp/longjmp.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include "src/__support/macros/properties/cpu_features.h"
#include "src/setjmp/wasm32/wasm_sjlj.h"

namespace LIBC_NAMESPACE_DECL {

#if defined(LIBC_TARGET_CPU_HAS_WASM_SJLJ)

// The real longjmp is produced by the backend pass; reaching this definition
// means the pass did not run, which is a build misconfiguration.
LLVM_LIBC_FUNCTION(void, longjmp, (jmp_buf, int)) { __builtin_trap(); }

} // namespace LIBC_NAMESPACE_DECL

// Runtime support called by the compiler-generated longjmp lowering. Must have
// external C linkage with this exact name.
extern "C" void __wasm_longjmp(void *env, int val) {
  LIBC_NAMESPACE::wasm_sjlj::jmp_buf_impl *jb =
      static_cast<LIBC_NAMESPACE::wasm_sjlj::jmp_buf_impl *>(env);
  // C standard: longjmp cannot make setjmp return 0; if val is 0 use 1.
  if (val == 0)
    val = 1;
  jb->arg.env = env;
  jb->arg.val = val;
  __builtin_wasm_throw(1, &jb->arg); // 1 == C_LONGJMP
}

// The `throw` above references the `__c_longjmp` exception tag, which the
// backend always emits as an external symbol. The tag must be defined exactly
// once in the final link; define it here as a weak symbol so it coexists with
// any other provider (e.g. wasi-libc's libsetjmp.a). A tag is defined by
// giving it a label in addition to the `.tagtype` directive.
__asm__("\t.text\n"
        "\t.tagtype\t__c_longjmp i32\n"
        "\t.globl\t__c_longjmp\n"
        "\t.weak\t__c_longjmp\n"
        "__c_longjmp:\n");

#else // !LIBC_TARGET_CPU_HAS_WASM_SJLJ

LLVM_LIBC_FUNCTION(void, longjmp, (jmp_buf, int)) {
  // setjmp/longjmp require the WebAssembly exception-handling based sjlj
  // lowering; build with the wasm-enable-sjlj backend pass to enable them.
  __builtin_trap();
}

} // namespace LIBC_NAMESPACE_DECL

#endif // LIBC_TARGET_CPU_HAS_WASM_SJLJ
