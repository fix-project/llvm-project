//===-- Shared mutable state for WASI pthread -----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_PTHREAD_WASI_PTHREAD_WASI_STATE_H
#define LLVM_LIBC_SRC_PTHREAD_WASI_PTHREAD_WASI_STATE_H

#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

// Single-threaded thread-local storage emulation: one value per key.
constexpr unsigned PTHREAD_MAX_KEYS = 128;
extern __pthread_tss_dtor_t tss_dtors[PTHREAD_MAX_KEYS];
extern const void *tss_values[PTHREAD_MAX_KEYS];

// Name of the single implicit thread.
constexpr unsigned PTHREAD_NAME_MAX = 16;
extern char thread_name[PTHREAD_NAME_MAX];

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_PTHREAD_WASI_PTHREAD_WASI_STATE_H
