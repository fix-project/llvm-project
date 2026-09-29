//===-- Reactor startup code for WASI -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Entry point for wasm reactors (shared objects). When linking with
// -shared, the Clang driver passes `--entry _initialize` to wasm-ld, so this
// object must define and export `_initialize`. It runs the .init_array
// constructors exactly once, mirroring the wasi-libc behavior.
//
//===----------------------------------------------------------------------===//

extern "C" {

// Synthesized by wasm-ld to run .init_array constructors.
void __wasm_call_ctors(void);

static int initialized = 0;

__attribute__((visibility("default"), export_name("_initialize")))
void _initialize(void) {
  if (initialized)
    __builtin_trap();
  initialized = 1;
  __wasm_call_ctors();
}

} // extern "C"
