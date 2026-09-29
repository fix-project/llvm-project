//===-- WASI implementation of freeaddrinfo ---------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/netdb/freeaddrinfo.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(void, freeaddrinfo, (struct addrinfo * res)) {
  // getaddrinfo never returns a list on WASI, so there is nothing to free.
  (void)res;
}

} // namespace LIBC_NAMESPACE_DECL
