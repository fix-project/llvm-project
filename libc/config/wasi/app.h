//===-- Classes to capture properties of WASI applications -------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_CONFIG_WASI_APP_H
#define LLVM_LIBC_CONFIG_WASI_APP_H

#include "hdr/stdint_proxy.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

struct Args {
  uintptr_t argc;

  // Even if argc is zero, "argv[argc] shall be a null pointer"
  // (ISO C 5.1.2.2.1) so one is fine.
  uintptr_t argv[1];
};

// Data structure which captures properties of a WASI application.
struct AppProperties {
  // Command line arguments.
  Args *args;

  // Environment data.
  uintptr_t *env_ptr;
};

[[gnu::weak]] extern AppProperties app;

} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_CONFIG_WASI_APP_H
