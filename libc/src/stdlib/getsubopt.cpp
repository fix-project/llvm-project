//===-- POSIX getsubopt ----------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdlib/getsubopt.h"
#include "src/__support/common.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, getsubopt,
                   (char **optionp, char *const *tokens, char **valuep)) {
  char *begin = *optionp;
  char *end = begin;
  while (*end != '\0' && *end != ',')
    ++end;

  char *value = begin;
  while (value < end && *value != '=')
    ++value;
  bool has_value = value < end;

  int match = -1;
  for (int i = 0; tokens[i] != nullptr; ++i) {
    const char *name = tokens[i];
    char *part = begin;
    while (part < value && *name != '\0' && *part == *name) {
      ++part;
      ++name;
    }
    if (part == value && *name == '\0') {
      match = i;
      break;
    }
  }

  if (*end == ',')
    *end++ = '\0';
  *optionp = end;
  if (match >= 0)
    *valuep = has_value ? value + 1 : nullptr;
  else
    *valuep = begin;
  return match;
}

} // namespace LIBC_NAMESPACE_DECL
