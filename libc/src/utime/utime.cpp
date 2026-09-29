//===-- Portable implementation of utime ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/utime/utime.h"

#include "hdr/types/struct_timeval.h"
#include "src/__support/common.h"
#include "src/sys/time/utimes.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, utime,
                   (const char *path, const struct utimbuf *times)) {
  if (times == nullptr)
    return utimes(path, nullptr);
  struct timeval tv[2] = {{times->actime, 0}, {times->modtime, 0}};
  return utimes(path, tv);
}

} // namespace LIBC_NAMESPACE_DECL
