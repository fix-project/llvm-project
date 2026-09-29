//===-- alloca.h -----------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LLVM_LIBC_ALLOCA_H
#define _LLVM_LIBC_ALLOCA_H

// Stack allocation must happen in the caller's frame, so this is a macro
// rather than a libc function.
#define alloca(size) __builtin_alloca(size)

#endif // _LLVM_LIBC_ALLOCA_H
