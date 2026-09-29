//===-- POSIX putenv -------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/stdlib/putenv.h"
#include "src/__support/CPP/string_view.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/null_check.h"
#include "src/stdlib/environ_internal.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, putenv, (char *entry)) {
  LIBC_CRASH_ON_NULLPTR(entry);
  cpp::string_view assignment(entry);
  size_t equal = assignment.find_first_of('=');
  if (equal == 0 || assignment.empty()) {
    libc_errno = EINVAL;
    return -1;
  }

  internal::EnvironmentManager &env =
      internal::EnvironmentManager::get_instance();
  // A name without '=' removes the variable on common Unix libcs.
  int result = equal == cpp::string_view::npos
                   ? env.unset(assignment)
                   : env.put(assignment.substr(0, equal), entry);
  if (result != 0)
    libc_errno = ENOMEM;
  return result;
}

} // namespace LIBC_NAMESPACE_DECL
