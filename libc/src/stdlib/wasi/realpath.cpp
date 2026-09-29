//===-- WASI implementation of realpath -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdlib/realpath.h"

#include "src/__support/CPP/string.h"
#include "src/__support/OSUtil/wasi/path.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(char *, realpath,
                   (const char *__restrict path,
                    char *__restrict resolved_path)) {
  char buf[wasi::PATH_MAX_SIZE];
  int err = wasi::canonicalize_path(path, buf, sizeof(buf), true);
  if (err != 0) {
    libc_errno = err;
    return nullptr;
  }

  if (resolved_path == nullptr) {
    cpp::string result(buf);
    return result.release_c_str();
  }
  size_t i = 0;
  do {
    resolved_path[i] = buf[i];
  } while (buf[i++] != '\0');
  return resolved_path;
}

} // namespace LIBC_NAMESPACE_DECL
