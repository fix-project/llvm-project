//===-- Device number helpers --------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_INCLUDE_SYS_SYSMACROS_H
#define LLVM_LIBC_INCLUDE_SYS_SYSMACROS_H

#include <stdint.h>

static inline unsigned int major(uint64_t device) {
  return (unsigned int)(((device >> 8) & 0xfff) | ((device >> 32) & ~0xfffULL));
}

static inline unsigned int minor(uint64_t device) {
  return (unsigned int)((device & 0xff) | ((device >> 12) & ~0xffULL));
}

static inline uint64_t makedev(unsigned int major_number,
                               unsigned int minor_number) {
  return (minor_number & 0xffULL) | ((major_number & 0xfffULL) << 8) |
         ((uint64_t)(minor_number & ~0xffU) << 12) |
         ((uint64_t)(major_number & ~0xfffU) << 32);
}

#endif // LLVM_LIBC_INCLUDE_SYS_SYSMACROS_H
