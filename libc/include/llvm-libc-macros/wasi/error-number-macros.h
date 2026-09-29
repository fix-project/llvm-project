//===-- WASI error number macros ------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_WASI_ERROR_NUMBER_MACROS_H
#define LLVM_LIBC_MACROS_WASI_ERROR_NUMBER_MACROS_H

// The following values match the WASI libc ABI (asm-generic/musl values).

#ifndef EDEADLK
#define EDEADLK 35
#endif // EDEADLK

#ifndef ENAMETOOLONG
#define ENAMETOOLONG 36
#endif // ENAMETOOLONG

#ifndef ENOLCK
#define ENOLCK 37
#endif // ENOLCK

#ifndef ENOSYS
#define ENOSYS 38
#endif // ENOSYS

#ifndef ENOTEMPTY
#define ENOTEMPTY 39
#endif // ENOTEMPTY

#ifndef ELOOP
#define ELOOP 40
#endif // ELOOP

#ifndef ENOMSG
#define ENOMSG 42
#endif // ENOMSG

#ifndef EIDRM
#define EIDRM 43
#endif // EIDRM

#ifndef EBADMSG
#define EBADMSG 74
#endif // EBADMSG

#ifndef EOVERFLOW
#define EOVERFLOW 75
#endif // EOVERFLOW

#ifndef ENOTSOCK
#define ENOTSOCK 88
#endif // ENOTSOCK

#ifndef EDESTADDRREQ
#define EDESTADDRREQ 89
#endif // EDESTADDRREQ

#ifndef EMSGSIZE
#define EMSGSIZE 90
#endif // EMSGSIZE

#ifndef EPROTOTYPE
#define EPROTOTYPE 91
#endif // EPROTOTYPE

#ifndef ENOPROTOOPT
#define ENOPROTOOPT 92
#endif // ENOPROTOOPT

#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT 93
#endif // EPROTONOSUPPORT

#ifndef EOPNOTSUPP
#define EOPNOTSUPP 95
#endif // EOPNOTSUPP

#ifndef ENOTSUP
#define ENOTSUP EOPNOTSUPP
#endif // ENOTSUP

#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT 97
#endif // EAFNOSUPPORT

#ifndef EADDRINUSE
#define EADDRINUSE 98
#endif // EADDRINUSE

#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL 99
#endif // EADDRNOTAVAIL

#ifndef ENETDOWN
#define ENETDOWN 100
#endif // ENETDOWN

#ifndef ENETUNREACH
#define ENETUNREACH 101
#endif // ENETUNREACH

#ifndef ENETRESET
#define ENETRESET 102
#endif // ENETRESET

#ifndef ECONNABORTED
#define ECONNABORTED 103
#endif // ECONNABORTED

#ifndef ECONNRESET
#define ECONNRESET 104
#endif // ECONNRESET

#ifndef ENOBUFS
#define ENOBUFS 105
#endif // ENOBUFS

#ifndef EISCONN
#define EISCONN 106
#endif // EISCONN

#ifndef ENOTCONN
#define ENOTCONN 107
#endif // ENOTCONN

#ifndef ETIMEDOUT
#define ETIMEDOUT 110
#endif // ETIMEDOUT

#ifndef ECONNREFUSED
#define ECONNREFUSED 111
#endif // ECONNREFUSED

#ifndef EHOSTUNREACH
#define EHOSTUNREACH 113
#endif // EHOSTUNREACH

#ifndef EALREADY
#define EALREADY 114
#endif // EALREADY

#ifndef EINPROGRESS
#define EINPROGRESS 115
#endif // EINPROGRESS

#ifndef ESTALE
#define ESTALE 116
#endif // ESTALE

#ifndef EDQUOT
#define EDQUOT 122
#endif // EDQUOT

#ifndef ECANCELED
#define ECANCELED 125
#endif // ECANCELED

#ifndef EOWNERDEAD
#define EOWNERDEAD 130
#endif // EOWNERDEAD

#ifndef ENOTRECOVERABLE
#define ENOTRECOVERABLE 131
#endif // ENOTRECOVERABLE

#ifndef ENOLINK
#define ENOLINK 67
#endif // ENOLINK

#ifndef EPROTO
#define EPROTO 71
#endif // EPROTO

#endif // LLVM_LIBC_MACROS_WASI_ERROR_NUMBER_MACROS_H
