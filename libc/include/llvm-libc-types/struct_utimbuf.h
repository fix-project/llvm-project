//===-- Definition of struct utimbuf -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_TYPES_STRUCT_UTIMBUF_H
#define LLVM_LIBC_TYPES_STRUCT_UTIMBUF_H

#include "time_t.h"

struct utimbuf {
  time_t actime;
  time_t modtime;
};

#endif // LLVM_LIBC_TYPES_STRUCT_UTIMBUF_H
