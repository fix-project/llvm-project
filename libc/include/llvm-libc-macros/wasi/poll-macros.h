//===-- Definition of macros from poll.h ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// WASI specific declarations of macros from poll.h. The values match the
/// WASI libc ABI. POLLPRI, POLLRDBAND and POLLWRBAND have no WASI libc
/// counterpart; they are accepted but carry no additional meaning.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_WASI_POLL_MACROS_H
#define LLVM_LIBC_MACROS_WASI_POLL_MACROS_H

#define POLLRDNORM 0x1
#define POLLWRNORM 0x2
#define POLLPRI 0x4
#define POLLRDBAND 0x8
#define POLLWRBAND 0x10

#define POLLIN POLLRDNORM
#define POLLOUT POLLWRNORM

#define POLLERR 0x1000
#define POLLHUP 0x2000
#define POLLNVAL 0x4000

#endif // LLVM_LIBC_MACROS_WASI_POLL_MACROS_H
