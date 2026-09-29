//===-- WASI entry point for a three-argument main -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/environ.h"

namespace LIBC_NAMESPACE_DECL {

// Clang names a two-argument WebAssembly main __main_argc_argv, but leaves a
// three-argument main named main. Keep this fallback in libc.a so the linker
// pulls it in only when no two-argument main supplies __main_argc_argv.
extern "C" int main_with_env(int, char **, char **) __asm__("main");

extern "C" int __main_argc_argv(int argc, char **argv) {
  return main_with_env(argc, argv, environ);
}

} // namespace LIBC_NAMESPACE_DECL
