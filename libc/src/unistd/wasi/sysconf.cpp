//===-- WASI implementation of sysconf ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/sysconf.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

#include "hdr/unistd_macros.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(long, sysconf, (int name)) {
  switch (name) {
  case _SC_VERSION:
    return 200809L;
  case _SC_THREADS:
    return _POSIX_THREADS;
  case _SC_ARG_MAX:
    return 131072L;
  case _SC_PHYS_PAGES:
    // Report the initial linear memory size (1 MiB pages).
    return static_cast<long>(__builtin_wasm_memory_size(0));
  case _SC_PAGESIZE:
    return 65536L;
  case _SC_CLK_TCK:
    // WASI clocks have nanosecond resolution.
    return 1000000000L;
  case _SC_OPEN_MAX:
    return 1024L;
  case _SC_NPROCESSORS_CONF:
  case _SC_NPROCESSORS_ONLN:
    return 1L;
  case _SC_SYMLOOP_MAX:
    return 32L;
  case _SC_HOST_NAME_MAX:
    return 255L;
  case _SC_LOGIN_NAME_MAX:
    return 256L;
  case _SC_TTY_NAME_MAX:
    return 32L;
  case _SC_GETPW_R_SIZE_MAX:
  case _SC_GETGR_R_SIZE_MAX:
    return -1L;
  default:
    libc_errno = EINVAL;
    return -1L;
  }
}

} // namespace LIBC_NAMESPACE_DECL
