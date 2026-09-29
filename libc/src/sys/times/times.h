//===-- Implementation header for times ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC_SYS_TIMES_TIMES_H
#define LLVM_LIBC_SRC_SYS_TIMES_TIMES_H

#include "llvm-libc-types/clock_t.h"
#include "llvm-libc-types/struct_tms.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

clock_t times(struct tms *buf);

} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC_SYS_TIMES_TIMES_H
