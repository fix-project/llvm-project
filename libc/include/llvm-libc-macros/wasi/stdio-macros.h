//===-- WASI macros from stdio.h -----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_MACROS_WASI_STDIO_MACROS_H
#define LLVM_LIBC_MACROS_WASI_STDIO_MACROS_H

#define FILENAME_MAX 4096
#define FOPEN_MAX 1000
#define L_tmpnam FILENAME_MAX
#define TMP_MAX 10000
#define P_tmpdir "."

#endif // LLVM_LIBC_MACROS_WASI_STDIO_MACROS_H
