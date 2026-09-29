//===-- Definition of shared WASI pthread state ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "pthread_wasi_state.h"

#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {
namespace wasi {

__pthread_tss_dtor_t tss_dtors[PTHREAD_MAX_KEYS];
const void *tss_values[PTHREAD_MAX_KEYS];
char thread_name[PTHREAD_NAME_MAX] = "main";

} // namespace wasi
} // namespace LIBC_NAMESPACE_DECL
