//===-- Definition of type struct dirent ----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_TYPES_STRUCT_DIRENT_H
#define LLVM_LIBC_TYPES_STRUCT_DIRENT_H

#include "ino_t.h"
#include "off_t.h"

struct dirent {
#ifdef __wasi__
  // Layout matching the WASI preview1 dirent record returned by fd_readdir.
  // The fields are explicitly sized because ino_t is 32-bit on wasm32 while
  // the WASI record uses 64-bit values for d_next and d_ino.
  unsigned long long d_next;
  unsigned long long d_ino;
  unsigned int d_namlen;
  unsigned char d_type;
  // WASI fd_readdir records place the name at offset 24.
  unsigned char __d_padding[3];
#elif defined(__linux__)
  ino_t d_ino;
  off_t d_off;
  unsigned short d_reclen;
  unsigned char d_type;
#else
  ino_t d_ino;
  unsigned char d_type;
#endif
  // The user code should use strlen to determine actual the size of d_name.
  // Likewise, it is incorrect and prohibited by the POSIX standard to detemine
  // the size of struct dirent type using sizeof. The size should be got using
  // a different method, for example, from the d_reclen field on Linux.
  char d_name[1];
};

#endif // LLVM_LIBC_TYPES_STRUCT_DIRENT_H
