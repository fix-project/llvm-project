//===-- Definition of macros from fcntl.h ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The values match the WASI libc ABI so that objects compiled against
// wasi-libc headers remain binary compatible. The low bits of the O_*
// flags map directly onto WASI fdflags, bits 12-15 map onto WASI oflags,
// and the top bits encode access modes and WASI-specific behaviors.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_WASI_FCNTL_MACROS_H
#define LLVM_LIBC_MACROS_WASI_FCNTL_MACROS_H

// Flags matching WASI fdflags (low byte).
#define O_APPEND (1 << 0)
#define O_DSYNC (1 << 1)
#define O_NONBLOCK (1 << 2)
#define O_RSYNC (1 << 3)
#define O_SYNC (1 << 4)

// Flags matching WASI oflags (bits 12-15).
#define O_CREAT (1 << 12)
#define O_DIRECTORY (1 << 13)
#define O_EXCL (1 << 14)
#define O_TRUNC (1 << 15)

// Access modes and behavior flags encoded in the top byte.
#define O_NOFOLLOW (0x01000000)
#define O_PATH (0x200000)
#define O_EXEC (0x02000000)
#define O_RDONLY (0x04000000)
#define O_SEARCH (0x08000000)
#define O_WRONLY (0x10000000)

#define O_RDWR (O_RDONLY | O_WRONLY)
#define O_ACCMODE (O_EXEC | O_RDWR | O_SEARCH)

// WASI has no exec-style functions; O_CLOEXEC carries no state.
#define O_CLOEXEC (0)
#define O_TTY_INIT (0)
#define O_NOCTTY (0)

// POSIX file advisory values map onto WASI advice codes.
#define POSIX_FADV_DONTNEED 4
#define POSIX_FADV_NOREUSE 5
#define POSIX_FADV_NORMAL 0
#define POSIX_FADV_RANDOM 1
#define POSIX_FADV_SEQUENTIAL 2
#define POSIX_FADV_WILLNEED 3

#define F_DUPFD (0)
#define F_GETFD (1)
#define F_SETFD (2)
#define F_GETFL (3)
#define F_SETFL (4)
#define F_GETLK (5)
#define F_SETLK (6)
#define F_SETLKW (7)
#define F_GETLK64 F_GETLK
#define F_SETLK64 F_SETLK
#define F_SETLKW64 F_SETLKW
#define F_SETOWN (8)
#define F_GETOWN (9)
#define F_DUPFD_CLOEXEC (1030)

#define F_RDLCK 0
#define F_WRLCK 1
#define F_UNLCK 2

#define FD_CLOEXEC (1)

#define AT_FDCWD (-2)
#define AT_EACCESS (0x0)
#define AT_SYMLINK_NOFOLLOW (0x1)
#define AT_SYMLINK_FOLLOW (0x2)
#define AT_REMOVEDIR (0x4)
#define AT_SYMLINK_EMPTY (0x800)
#define AT_EMPTY_PATH (0x1000)

#endif // LLVM_LIBC_MACROS_WASI_FCNTL_MACROS_H
