//===-- WASI callonce fastpath --------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_THREADS_WASI_CALLONCE_H
#define LLVM_LIBC_SRC___SUPPORT_THREADS_WASI_CALLONCE_H

#include "src/__support/macros/config.h"
#include "src/__support/macros/optimization.h" // LIBC_LIKELY

namespace LIBC_NAMESPACE_DECL {

// WASI is single-threaded, so a simple flag is sufficient. The underlying
// representation matches the one used by the other supported platforms so
// that the public "once_flag" type stays the same.
struct CallOnceFlag {
  unsigned __word;
};

namespace callonce_impl {
static constexpr unsigned NOT_CALLED = 0x0;
static constexpr unsigned FINISH = 0x33;

LIBC_INLINE bool callonce_fastpath(CallOnceFlag *flag) {
  return flag->__word == FINISH;
}

template <class CallOnceCallback>
[[gnu::noinline, gnu::cold]] int callonce_slowpath(CallOnceFlag *flag,
                                                   CallOnceCallback callback) {
  if (flag->__word != FINISH) {
    callback();
    flag->__word = FINISH;
  }
  return 0;
}
} // namespace callonce_impl

} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_THREADS_WASI_CALLONCE_H
