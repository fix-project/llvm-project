//===-- WASI implementation of uname --------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/sys/utsname/uname.h"

#include "hdr/types/size_t.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"

#include <sys/utsname.h>

namespace LIBC_NAMESPACE_DECL {

namespace {

void copy_str(char *dst, size_t size, const char *src) {
  size_t i = 0;
  for (; i + 1 < size && src[i] != '\0'; ++i)
    dst[i] = src[i];
  for (; i < size; ++i)
    dst[i] = '\0';
}

} // namespace

LLVM_LIBC_FUNCTION(int, uname, (struct utsname * name)) {
  copy_str(name->sysname, sizeof(name->sysname), "wasi");
  copy_str(name->nodename, sizeof(name->nodename), "wasi");
  copy_str(name->release, sizeof(name->release), "0");
  copy_str(name->version, sizeof(name->version), "0");
#if defined(__wasm64__)
  copy_str(name->machine, sizeof(name->machine), "wasm64");
#else
  copy_str(name->machine, sizeof(name->machine), "wasm32");
#endif
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
