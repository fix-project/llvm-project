//===-- Shared stub helpers for WASI pthread ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_PTHREAD_WASI_PTHREAD_WASI_H
#define LLVM_LIBC_SRC_PTHREAD_WASI_PTHREAD_WASI_H

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

// WASI is single-threaded: thread creation always fails.
constexpr int pthread_enotsup() {
  return ENOTSUP;
}

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_PTHREAD_WASI_PTHREAD_WASI_H