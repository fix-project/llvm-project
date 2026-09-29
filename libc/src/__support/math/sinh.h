//===-- Inline sinh function -----------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_MATH_SINH_H
#define LLVM_LIBC_SRC___SUPPORT_MATH_SINH_H

#include "exp.h"
#include "src/__support/FPUtil/FEnvImpl.h"
#include "src/__support/FPUtil/FPBits.h"
#include "src/__support/FPUtil/multiply_add.h"
#include "src/__support/macros/config.h"
#include "src/__support/macros/optimization.h" // LIBC_UNLIKELY

namespace LIBC_NAMESPACE_DECL {
namespace math {

LIBC_INLINE double sinh(double x) {
  static constexpr double SINH_POLY[] = {
    0x1.0000000000002p+0,
    0x1.55555555551d7p-3,
    0x1.111111113be93p-7,
    0x1.a01a016a4924ap-13,
    0x1.71de5b9b6db6dp-19,
    0x1.ae4f000000003p-26,
    0x1.67edb6db6db6ep-33,
  };

  using FPBits = fputil::FPBits<double>;
  const uint64_t bits = FPBits(x).uintval();
  const uint64_t x_abs = bits & 0x7fff'ffff'ffff'ffffULL;
  const bool negative = bits >> 63;

  // |x| < 2^-27: sinh(x) ~= x + x^3/6
  if (LIBC_UNLIKELY(x_abs < 0x3e40'0000'0000'0000ULL)) {
    if (LIBC_UNLIKELY(x_abs < 0x0010'0000'0000'0000ULL)) {
      fputil::raise_except_if_required(FE_UNDERFLOW);
      fputil::set_errno_if_required(ERANGE);
    }
    const double x2 = x * x;
    return fputil::multiply_add(x2 * x, 0x1.5555555555555p-3, x);
  }

  // inf and nan
  if (LIBC_UNLIKELY(x_abs >= 0x7ff0'0000'0000'0000ULL)) {
    if (x_abs > 0x7ff0'0000'0000'0000ULL) {
      fputil::raise_except_if_required(FE_INVALID);
      return FPBits::quiet_nan().get_val();
    }
    return x;
  }

  // Overflow: |x| > asinh(DBL_MAX) ~= 710.4758600739434
  if (LIBC_UNLIKELY(x_abs > 0x4086'2e42'fe18'ae8bULL)) {
    fputil::raise_except_if_required(FE_OVERFLOW);
    fputil::set_errno_if_required(ERANGE);
    return negative ? -FPBits::inf().get_val() : FPBits::inf().get_val();
  }

  const double ax = negative ? -x : x;

  if (LIBC_UNLIKELY(x_abs <= 0x3ff0'0000'0000'0000ULL)) { // |x| <= 1
    const double t = ax * ax;
    double r = SINH_POLY[6];
    r = fputil::multiply_add(r, t, SINH_POLY[5]);
    r = fputil::multiply_add(r, t, SINH_POLY[4]);
    r = fputil::multiply_add(r, t, SINH_POLY[3]);
    r = fputil::multiply_add(r, t, SINH_POLY[2]);
    r = fputil::multiply_add(r, t, SINH_POLY[1]);
    r = fputil::multiply_add(r, t, SINH_POLY[0]);
    const double res = ax * r;
    return negative ? -res : res;
  }

  if (x_abs < 0x4036'0000'0000'0000ULL) { // |x| < 22
    const double t = math::exp(ax);
    const double res = 0.5 * (t - 1.0 / t);
    return negative ? -res : res;
  }

  // 22 <= |x| <= asinh(DBL_MAX): sinh(|x|) = 0.5 * exp(|x|)
  const double res = math::exp(ax - 1.0) * 0x1.5b0e'dec46332p+0; // e/2
  return negative ? -res : res;
}

} // namespace math
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_MATH_SINH_H
