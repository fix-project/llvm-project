//===-- WASI implementation of IO utils ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_IO_H
#define LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_IO_H

#include "src/__support/CPP/string_view.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/macros/attributes.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LIBC_INLINE void write_to_stderr(cpp::string_view msg) {
  wasi::__wasi_ciovec_t iov = {const_cast<char *>(msg.data()),
                               static_cast<wasi::__wasi_size_t>(msg.size())};
  wasi::__wasi_size_t nwritten = 0;
  wasi::__wasi_fd_write(wasi::__WASI_STDERR_FILENO, &iov, 1, &nwritten);
}

} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_OSUTIL_WASI_IO_H
