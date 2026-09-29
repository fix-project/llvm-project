//===-- Inline cosh function -----------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_MATH_COSH_H
#define LLVM_LIBC_SRC___SUPPORT_MATH_COSH_H

#include "exp.h"
#include "src/__support/FPUtil/FEnvImpl.h"
#include "src/__support/FPUtil/FPBits.h"
#include "src/__support/FPUtil/multiply_add.h"
#include "src/__support/macros/config.h"
#include "src/__support/macros/optimization.h" // LIBC_UNLIKELY

namespace LIBC_NAMESPACE_DECL {
namespace math {

LIBC_INLINE double cosh(double x) {
  static constexpr double COSH_POLY[] = {
    0x1.0000000000007p+0,
    0x1.ffffffffff5a6p-2,
    0x1.55555555a72dbp-5,
    0x1.6c16c12f636dcp-10,
    0x1.a01a2c8b6db6fp-16,
    0x1.27d565b6db6ddp-22,
    0x1.2489249249249p-29,
  };

  using FPBits = fputil::FPBits<double>;
  const uint64_t bits = FPBits(x).uintval();
  const uint64_t x_abs = bits & 0x7fff'ffff'ffff'ffffULL;

  // |x| < 2^-27: cosh(x) ~= 1 + x^2/2
  if (LIBC_UNLIKELY(x_abs < 0x3e40'0000'0000'0000ULL))
    return 1.0 + 0.5 * x * x;

  // inf and nan
  if (LIBC_UNLIKELY(x_abs >= 0x7ff0'0000'0000'0000ULL)) {
    if (x_abs > 0x7ff0'0000'0000'0000ULL) {
      fputil::raise_except_if_required(FE_INVALID);
      return FPBits::quiet_nan().get_val();
    }
    return FPBits::inf().get_val();
  }

  // Overflow: |x| > acosh(DBL_MAX) ~= 710.4758600739434
  if (LIBC_UNLIKELY(x_abs > 0x4086'2e42'fe18'ae8bULL)) {
    fputil::raise_except_if_required(FE_OVERFLOW);
    fputil::set_errno_if_required(ERANGE);
    return FPBits::inf().get_val();
  }

  const double ax = (bits >> 63) ? -x : x;

  if (LIBC_UNLIKELY(x_abs <= 0x3ff0'0000'0000'0000ULL)) { // |x| <= 1
    const double t = ax * ax;
    double r = COSH_POLY[6];
    r = fputil::multiply_add(r, t, COSH_POLY[5]);
    r = fputil::multiply_add(r, t, COSH_POLY[4]);
    r = fputil::multiply_add(r, t, COSH_POLY[3]);
    r = fputil::multiply_add(r, t, COSH_POLY[2]);
    r = fputil::multiply_add(r, t, COSH_POLY[1]);
    r = fputil::multiply_add(r, t, COSH_POLY[0]);
    return r;
  }

  if (x_abs < 0x4036'0000'0000'0000ULL) { // |x| < 22
    const double t = math::exp(ax);
    return 0.5 * (t + 1.0 / t);
  }

  // 22 <= |x| <= acosh(DBL_MAX): cosh(|x|) = 0.5 * exp(|x|)
  return math::exp(ax - 1.0) * 0x1.5b0e'dec46332p+0; // e/2
}

} // namespace math
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_MATH_COSH_H
