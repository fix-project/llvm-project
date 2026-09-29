//===-- WASI system logging -----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdio/fputc.h"
#include "src/stdio/fputs.h"
#include "src/stdio/stderr.h"
#include "src/stdio/vfprintf.h"
#include "src/syslog/syslog.h"
#include "src/__support/common.h"
#include "src/__support/macros/config.h"

#include <stdarg.h>
#include <syslog.h>

namespace LIBC_NAMESPACE_DECL {
namespace {
const char *ident = nullptr;
int log_mask = LOG_UPTO(LOG_DEBUG);
} // namespace

LLVM_LIBC_FUNCTION(void, openlog,
                   (const char *new_ident, int, int)) {
  ident = new_ident;
}

LLVM_LIBC_FUNCTION(void, closelog, (void)) { ident = nullptr; }

LLVM_LIBC_FUNCTION(int, setlogmask, (int mask)) {
  int old_mask = log_mask;
  if (mask != 0)
    log_mask = mask;
  return old_mask;
}

LLVM_LIBC_FUNCTION(void, vsyslog,
                   (int priority, const char *format, va_list args)) {
  if (priority < 0 || !(log_mask & LOG_MASK(LOG_PRI(priority))))
    return;
  if (ident != nullptr) {
    fputs(ident, stderr);
    fputs(": ", stderr);
  }
  vfprintf(stderr, format, args);
  fputc('\n', stderr);
}

LLVM_LIBC_FUNCTION(void, syslog, (int priority, const char *format, ...)) {
  va_list args;
  va_start(args, format);
  vsyslog(priority, format, args);
  va_end(args);
}
} // namespace LIBC_NAMESPACE_DECL
