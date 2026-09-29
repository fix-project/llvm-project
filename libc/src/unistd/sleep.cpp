//===-- Portable implementation of sleep ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/unistd/sleep.h"

#include "hdr/types/struct_timespec.h"
#include "src/__support/common.h"
#include "src/time/nanosleep.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(unsigned int, sleep, (unsigned int seconds)) {
  timespec request = {static_cast<time_t>(seconds), 0};
  timespec remaining = {};
  if (nanosleep(&request, &remaining) == 0)
    return 0;
  return static_cast<unsigned int>(remaining.tv_sec + (remaining.tv_nsec != 0));
}

} // namespace LIBC_NAMESPACE_DECL
