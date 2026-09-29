//===-- WASI implementation of getnameinfo ----------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/netdb/eai_codes.h"
#include "src/netdb/getnameinfo.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, getnameinfo,
                   (const struct sockaddr *__restrict sa, socklen_t salen,
                    char *__restrict host, socklen_t hostlen,
                    char *__restrict serv, socklen_t servlen, int flags)) {
  (void)sa;
  (void)salen;
  (void)host;
  (void)hostlen;
  (void)serv;
  (void)servlen;
  (void)flags;
  // WASI does not provide name resolution facilities.
  libc_errno = ENOSYS;
  return netdb::EAI_SYSTEM;
}

} // namespace LIBC_NAMESPACE_DECL
