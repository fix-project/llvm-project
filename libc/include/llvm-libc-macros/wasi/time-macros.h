//===-- WASI time.h / sys/time.h macros ------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_WASI_TIME_MACROS_H
#define LLVM_LIBC_MACROS_WASI_TIME_MACROS_H

// The WASI clock ids coincide with the POSIX clock ids exposed by
// wasi-libc / musl for the first four clocks.
#define CLOCKS_PER_SEC 1000000
#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1
#define CLOCK_PROCESS_CPUTIME_ID 2
#define CLOCK_THREAD_CPUTIME_ID 3

#endif // LLVM_LIBC_MACROS_WASI_TIME_MACROS_H
