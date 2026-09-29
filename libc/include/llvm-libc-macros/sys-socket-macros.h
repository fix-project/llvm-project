//===-- Macros defined in sys/socket.h header file ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_SYS_SOCKET_MACROS_H
#define LLVM_LIBC_MACROS_SYS_SOCKET_MACROS_H

#ifdef __linux__
#include "linux/sys-socket-macros.h"
#elif defined(__wasi__)
#define AF_UNSPEC 0
#define AF_UNIX 1
#define AF_INET 2   // Internet IPv4 Protocol
#define AF_INET6 10 // IP version 6
#define PF_UNSPEC AF_UNSPEC
#define PF_UNIX AF_UNIX
#define PF_INET AF_INET
#define PF_INET6 AF_INET6

#define SOCK_STREAM 1
#define SOCK_DGRAM 2
#define SOCK_RAW 3
#define SOCK_RDM 4
#define SOCK_SEQPACKET 5
#define SOCK_PACKET 10
#define SOCK_CLOEXEC 0x80000
#define SOCK_NONBLOCK 0x800

#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2
#endif

#endif // LLVM_LIBC_MACROS_SYS_SOCKET_MACROS_H
