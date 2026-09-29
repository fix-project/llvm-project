//===-- WASI implementation of getaddrinfo ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/netdb/eai_codes.h"
#include "src/netdb/getaddrinfo.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, getaddrinfo,
                   (const char *__restrict node,
                    const char *__restrict service,
                    const struct addrinfo *__restrict hints,
                    struct addrinfo **__restrict res)) {
  (void)node;
  (void)service;
  (void)hints;
  (void)res;
  // WASI does not provide name resolution facilities.
  libc_errno = ENOSYS;
  return netdb::EAI_SYSTEM;
}

} // namespace LIBC_NAMESPACE_DECL
