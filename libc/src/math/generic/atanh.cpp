//===-- Double-precision atanh function -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/math/atanh.h"
#include "src/__support/FPUtil/FEnvImpl.h"
#include "src/__support/FPUtil/FPBits.h"
#include "src/__support/common.h"
#include "src/__support/math/log1p.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(double, atanh, (double x)) {
  using FPBits = fputil::FPBits<double>;
  FPBits bits(x);
  if (bits.is_nan())
    return x + x;
  double ax = bits.abs().get_val();
  if (ax > 1.0) {
    fputil::set_errno_if_required(EDOM);
    fputil::raise_except_if_required(FE_INVALID);
    return FPBits::quiet_nan().get_val();
  }
  if (ax == 1.0) {
    fputil::set_errno_if_required(ERANGE);
    fputil::raise_except_if_required(FE_DIVBYZERO);
    return FPBits::inf(bits.sign()).get_val();
  }
  if (ax < 0x1p-32)
    return x;
  double result = ax < 0.5
                      ? 0.5 * math::log1p(2.0 * ax + 2.0 * ax * ax / (1.0 - ax))
                      : 0.5 * math::log1p(2.0 * (ax / (1.0 - ax)));
  return bits.is_neg() ? -result : result;
}

} // namespace LIBC_NAMESPACE_DECL
