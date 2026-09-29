//===-- Definition of macros from threads.h ---------------------*- C -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_INCLUDE_LLVM_LIBC_MACROS_THREADS_MACROS_H
#define LLVM_LIBC_INCLUDE_LLVM_LIBC_MACROS_THREADS_MACROS_H

// C11 7.26.1p4: the macro thread_local expands to _Thread_local.  C++ has
// the thread_local keyword built in, so no macro is provided there.
#if !defined(__cplusplus)
#define thread_local _Thread_local
#endif

#define TSS_DTOR_ITERATIONS 4

#endif // LLVM_LIBC_INCLUDE_LLVM_LIBC_MACROS_THREADS_MACROS_H