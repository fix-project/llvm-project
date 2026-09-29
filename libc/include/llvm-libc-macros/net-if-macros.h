//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Macros defined in net/if.h header file.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_NET_IF_MACROS_H
#define LLVM_LIBC_MACROS_NET_IF_MACROS_H

#ifdef __linux__
#include "linux/net-if-macros.h"
#elif defined(__wasi__)
// POSIX specifies IF_NAMESIZE for if_indextoname's result buffer.
#define IF_NAMESIZE 16
#endif

#endif // LLVM_LIBC_MACROS_NET_IF_MACROS_H
