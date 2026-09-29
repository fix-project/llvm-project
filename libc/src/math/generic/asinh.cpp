//===-- Double-precision asinh function -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/math/asinh.h"
#include "src/__support/FPUtil/FPBits.h"
#include "src/__support/common.h"
#include "src/__support/math/log.h"
#include "src/__support/math/log1p.h"
#include "src/__support/math/sqrt.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(double, asinh, (double x)) {
  using FPBits = fputil::FPBits<double>;
  FPBits bits(x);
  if (bits.is_inf_or_nan())
    return x + x;
  double ax = bits.abs().get_val();
  if (ax < 0x1p-26)
    return x;
  double result;
  if (ax >= 0x1p26)
    result = math::log(ax) + 0x1.62e42fefa39efp-1;
  else if (ax >= 2.0)
    result = math::log(2.0 * ax + 1.0 / (math::sqrt(ax * ax + 1.0) + ax));
  else
    result = math::log1p(ax + ax * ax / (math::sqrt(ax * ax + 1.0) + 1.0));
  return bits.is_neg() ? -result : result;
}

} // namespace LIBC_NAMESPACE_DECL
