//===-- Implementation of wcwidth -----------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/wchar/wcwidth.h"

#include "src/__support/common.h"
#include "src/wctype/iswprint.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, wcwidth, (wchar_t wc)) {
  if (wc == 0)
    return 0;
  return LIBC_NAMESPACE::iswprint(static_cast<wint_t>(wc)) ? 1 : -1;
}

} // namespace LIBC_NAMESPACE_DECL
