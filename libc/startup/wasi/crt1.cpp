//===-- Implementation of crt for WASI ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "config/wasi/app.h"
#include "hdr/stdint_proxy.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/macros/config.h"
#include "src/stdlib/exit.h"
#include "src/stdlib/malloc.h"
#include "src/unistd/environ.h"

namespace LIBC_NAMESPACE_DECL {

AppProperties app;

namespace {

using namespace wasi;

[[noreturn]] void startup_error() {
  __wasi_proc_exit(1);
  __builtin_unreachable();
}

// The host supplies the counts and byte sizes before writing the values.
// Keep the resulting storage alive for constructors and main.
char **allocate_pointers(__wasi_size_t count) {
  if (count > __SIZE_MAX__ / sizeof(char *) - 1)
    startup_error();
  char **pointers = static_cast<char **>(malloc((count + 1) * sizeof(char *)));
  if (pointers == nullptr)
    startup_error();
  return pointers;
}

} // namespace

extern "C" {

// Provided by the linker; runs the .init_array constructors.
void __wasm_call_ctors();

// Command line captured before calling into user code.
int start_argc;
char **start_argv;

// Resolved either by a zero-argument main's alias or by the fallback in libc.a.
int __main_void();

[[gnu::visibility("default")]] [[gnu::used]] void _start() {
  __wasi_size_t argc = 0;
  __wasi_size_t argv_buf_size = 0;
  if (__wasi_args_sizes_get(&argc, &argv_buf_size) != __WASI_ERRNO_SUCCESS ||
      argc > __INT_MAX__ ||
      argc > (__SIZE_MAX__ - sizeof(Args)) / sizeof(uintptr_t))
    startup_error();

  __wasi_size_t envc = 0;
  __wasi_size_t envp_buf_size = 0;
  if (__wasi_environ_sizes_get(&envc, &envp_buf_size) != __WASI_ERRNO_SUCCESS)
    startup_error();

  char **argv_ptrs = allocate_pointers(argc);
  char *argv_buf = nullptr;
  if (argc > 0) {
    argv_buf = static_cast<char *>(malloc(argv_buf_size));
    if (argv_buf == nullptr ||
        __wasi_args_get(argv_ptrs, argv_buf) != __WASI_ERRNO_SUCCESS)
      startup_error();
  }
  argv_ptrs[argc] = nullptr;

  char **envp_ptrs = allocate_pointers(envc);
  char *envp_buf = nullptr;
  if (envc > 0) {
    envp_buf = static_cast<char *>(malloc(envp_buf_size));
    if (envp_buf == nullptr ||
        __wasi_environ_get(envp_ptrs, envp_buf) != __WASI_ERRNO_SUCCESS)
      startup_error();
  }
  envp_ptrs[envc] = nullptr;

  app.args =
      static_cast<Args *>(malloc(sizeof(Args) + argc * sizeof(uintptr_t)));
  if (app.args == nullptr)
    startup_error();
  app.args->argc = argc;
  uintptr_t *argv_field = app.args->argv;
  for (__wasi_size_t i = 0; i < argc; ++i)
    argv_field[i] = reinterpret_cast<uintptr_t>(argv_ptrs[i]);
  argv_field[argc] = 0;
  app.env_ptr = reinterpret_cast<uintptr_t *>(envp_ptrs);
  environ = reinterpret_cast<char **>(envp_ptrs);

  start_argc = static_cast<int>(argc);
  start_argv = argv_ptrs;

  __wasm_call_ctors();
  int status = __main_void();
  exit(status);
}
} // extern "C"

} // namespace LIBC_NAMESPACE_DECL
