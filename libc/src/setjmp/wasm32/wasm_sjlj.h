//===-- Shared layout for WebAssembly setjmp/longjmp ----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Layout of the state stored inside jmp_buf by the WebAssembly sjlj runtime.
// The compiler-generated code (WebAssemblyLowerEmscriptenEHSjLj) and the
// __wasm_* runtime helpers must agree on this layout.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SETJMP_WASM32_WASM_SJLJ_H
#define LLVM_LIBC_SRC_SETJMP_WASM32_WASM_SJLJ_H

#include <stdint.h>

namespace LIBC_NAMESPACE_DECL {
namespace wasm_sjlj {

struct arg {
  void *env;
  int val;
};

struct jmp_buf_impl {
  void *func_invocation_id;
  uint32_t label;
  arg arg;
};

} // namespace wasm_sjlj
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_SETJMP_WASM32_WASM_SJLJ_H
