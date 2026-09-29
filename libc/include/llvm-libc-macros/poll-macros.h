//===-- Macros defined in poll.h header file ------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_POLL_MACROS_H
#define LLVM_LIBC_MACROS_POLL_MACROS_H

#if defined(__linux__)
#include "linux/poll-macros.h"
#elif defined(__wasi__)
#include "wasi/poll-macros.h"
#endif

#endif // LLVM_LIBC_MACROS_POLL_MACROS_H
