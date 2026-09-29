//===-- WASI implementation of mkstemp ------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Implementation of mkstemp, a POSIX function that creates a unique temporary
/// file from a template string ending in at least six 'X' characters.
///
//===----------------------------------------------------------------------===//

#include "src/stdlib/mkstemp.h"

#include "hdr/errno_macros.h"
#include "hdr/fcntl_macros.h"
#include "src/__support/CPP/string_view.h"
#include "src/__support/OSUtil/wasi/wasi.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include "src/__support/macros/null_check.h"
#include "src/fcntl/wasi/open_utils.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, mkstemp, (char *tmpl)) {
  LIBC_CRASH_ON_NULLPTR(tmpl);

  cpp::string_view str_view(tmpl);
  size_t count = 0;
  size_t len = str_view.size();

  for (size_t i = len; i > 0; i--) {
    if (str_view[i - 1] != 'X')
      break;
    count++;
  }

  if (count < 6) {
    libc_errno = EINVAL;
    return -1;
  }

  char *suffix = tmpl + len - count;

  // POSIX portable filename character set, sorted by ASCII value.
  const char charset[] = "-._0123456789"
                         "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                         "abcdefghijklmnopqrstuvwxyz";

  int result = -1;
  bool file_created = false;
  while (!file_created) {
    uint8_t rand_bytes[64];
    wasi::__wasi_errno_t err = wasi::__wasi_random_get(
        rand_bytes, static_cast<wasi::__wasi_size_t>(sizeof(rand_bytes)));
    if (err != 0) {
      libc_errno = wasi::wasi_to_errno(err);
      return -1;
    }

    for (size_t i = 0; i < count; i++) {
      // sizeof(charset) - 1 to account for the null terminator
      suffix[i] = charset[rand_bytes[i % sizeof(rand_bytes)] %
                         (sizeof(charset) - 1)];
    }

    int fd = wasi::openat_impl(AT_FDCWD, tmpl, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (fd < 0) {
      if (-fd == EEXIST)
        continue;
      libc_errno = -fd;
      return -1;
    }
    result = fd;
    file_created = true;
  }

  return result;
}

} // namespace LIBC_NAMESPACE_DECL
