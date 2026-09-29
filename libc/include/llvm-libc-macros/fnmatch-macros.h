//===-- Macros for fnmatch.h ----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_FNMATCH_MACROS_H
#define LLVM_LIBC_MACROS_FNMATCH_MACROS_H

#define FNM_PATHNAME 0x01
#define FNM_NOESCAPE 0x02
#define FNM_PERIOD 0x04
#define FNM_LEADING_DIR 0x08
#define FNM_CASEFOLD 0x10
#define FNM_FILE_NAME FNM_PATHNAME
#define FNM_NOMATCH 1

#endif // LLVM_LIBC_MACROS_FNMATCH_MACROS_H
