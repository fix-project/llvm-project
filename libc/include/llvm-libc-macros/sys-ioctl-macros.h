//===-- Macros defined in sys/ioctl.h header file -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_SYS_IOCTL_MACROS_H
#define LLVM_LIBC_MACROS_SYS_IOCTL_MACROS_H

#ifdef __linux__
#include "linux/sys-ioctl-macros.h"
#elif defined(__wasi__)
#define TIOCGWINSZ 0x5413
#define TIOCNOTTY 0x5422
#endif

#endif // LLVM_LIBC_MACROS_SYS_IOCTL_MACROS_H
