//===-- Macros defined in sys/mman.h header file --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_SYS_MMAN_MACROS_H
#define LLVM_LIBC_MACROS_SYS_MMAN_MACROS_H

// Use definitions from <linux/mman.h> to dispatch arch-specific flag values.
// For example, MCL_CURRENT/MCL_FUTURE/MCL_ONFAULT are different on different
// architectures.
#if defined(__wasi__)
// WASI mappings are emulated with heap memory and file I/O.
#define PROT_NONE 0
#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4
#define MAP_SHARED 0x01
#define MAP_PRIVATE 0x02
#define MAP_FIXED 0x10
#define MAP_ANONYMOUS 0x20
#define MAP_ANON MAP_ANONYMOUS
#define MAP_FAILED ((void *) -1)
#define MS_ASYNC 1
#define MS_INVALIDATE 2
#define MS_SYNC 4
#define MCL_CURRENT 1
#define MCL_FUTURE 2
#define MCL_ONFAULT 4
#define MADV_NORMAL 0
#define MADV_RANDOM 1
#define MADV_SEQUENTIAL 2
#define MADV_WILLNEED 3
#define MADV_DONTNEED 4
#define MREMAP_MAYMOVE 1
#elif __has_include(<linux/mman.h>)
#include <linux/mman.h>
#else
#error "cannot use <sys/mman.h> without proper system headers."
#endif

#if __has_include(<linux/memfd.h>)
#include <linux/memfd.h>
#endif

// Some posix standard flags may not be defined in system headers.
// Posix mmap flags.
#ifndef MAP_FAILED
#define MAP_FAILED ((void *)-1)
#endif

// Posix memory advise flags. (posix_madvise)
#ifndef POSIX_MADV_NORMAL
#define POSIX_MADV_NORMAL MADV_NORMAL
#endif

#ifndef POSIX_MADV_SEQUENTIAL
#define POSIX_MADV_SEQUENTIAL MADV_SEQUENTIAL
#endif

#ifndef POSIX_MADV_RANDOM
#define POSIX_MADV_RANDOM MADV_RANDOM
#endif

#ifndef POSIX_MADV_WILLNEED
#define POSIX_MADV_WILLNEED MADV_WILLNEED
#endif

#ifndef POSIX_MADV_DONTNEED
#define POSIX_MADV_DONTNEED MADV_DONTNEED
#endif

#endif // LLVM_LIBC_MACROS_SYS_MMAN_MACROS_H
