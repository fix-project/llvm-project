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
#include "src/unistd/environ.h"

namespace LIBC_NAMESPACE_DECL {

AppProperties app;

namespace {

using namespace wasi;

// Static storage for the command line arguments and the environment. The
// sizes are generous but bounded; WASI hosts typically provide small
// environments and short command lines.
constexpr size_t MAX_ARGS = 256;
constexpr size_t MAX_ENV_VARS = 256;
constexpr size_t ARGV_BUF_SIZE = 16384;
constexpr size_t ENVP_BUF_SIZE = 32768;

alignas(16) char argv_buf[ARGV_BUF_SIZE];
char *argv_ptrs[MAX_ARGS + 1];
alignas(16) char envp_buf[ENVP_BUF_SIZE];
char *envp_ptrs[MAX_ENV_VARS + 1];

// Storage backing the flexible argv array of Args.
struct ArgsBlock {
  Args args;
  uintptr_t extra[MAX_ARGS];
};
ArgsBlock args_block;

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
      argc > MAX_ARGS || argv_buf_size > ARGV_BUF_SIZE) {
    argc = 0;
  }

  __wasi_size_t envc = 0;
  __wasi_size_t envp_buf_size = 0;
  if (__wasi_environ_sizes_get(&envc, &envp_buf_size) !=
          __WASI_ERRNO_SUCCESS ||
      envc > MAX_ENV_VARS || envp_buf_size > ENVP_BUF_SIZE) {
    envc = 0;
  }

  if (argc > 0 &&
      __wasi_args_get(reinterpret_cast<char ***>(argv_ptrs), argv_buf) !=
          __WASI_ERRNO_SUCCESS) {
    argc = 0;
  }
  argv_ptrs[argc] = nullptr;

  if (envc > 0 &&
      __wasi_environ_get(reinterpret_cast<char ***>(envp_ptrs), envp_buf) !=
          __WASI_ERRNO_SUCCESS) {
    envc = 0;
  }
  envp_ptrs[envc] = nullptr;

  app.args = &args_block.args;
  app.args->argc = argc;
  uintptr_t *argv_field = app.args->argv;
  for (__wasi_size_t i = 0; i < argc; ++i)
    argv_field[i] = reinterpret_cast<uintptr_t>(argv_ptrs[i]);
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
