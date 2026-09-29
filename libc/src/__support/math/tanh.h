//===-- Inline tanh function -----------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_MATH_TANH_H
#define LLVM_LIBC_SRC___SUPPORT_MATH_TANH_H

#include "exp.h"
#include "src/__support/FPUtil/FEnvImpl.h"
#include "src/__support/FPUtil/FPBits.h"
#include "src/__support/FPUtil/multiply_add.h"
#include "src/__support/macros/config.h"
#include "src/__support/macros/optimization.h" // LIBC_UNLIKELY

namespace LIBC_NAMESPACE_DECL {
namespace math {

LIBC_INLINE double tanh(double x) {
  static constexpr double TANH_POLY[] = {
    0x1.0000000000003p+0,
    -0x1.5555555555937p-2,
    0x1.1111111083e3ap-3,
    -0x1.ba1ba111dcd07p-5,
    0x1.664f24740343ep-6,
    -0x1.226a3955899a7p-7,
    0x1.d6509fb2bd117p-9,
    -0x1.788084d4830afp-10,
    0x1.1606d34d1ae2ap-11,
    -0x1.17c9d03acc590p-13,
  };

  using FPBits = fputil::FPBits<double>;
  const uint64_t bits = FPBits(x).uintval();
  const uint64_t x_abs = bits & 0x7fff'ffff'ffff'ffffULL;
  const bool negative = bits >> 63;

  // |x| < 2^-28: tanh(x) ~= x
  if (LIBC_UNLIKELY(x_abs < 0x3e30'0000'0000'0000ULL))
    return x;

  // inf and nan
  if (LIBC_UNLIKELY(x_abs >= 0x7ff0'0000'0000'0000ULL)) {
    if (x_abs > 0x7ff0'0000'0000'0000ULL) {
      fputil::raise_except_if_required(FE_INVALID);
      return FPBits::quiet_nan().get_val();
    }
    return negative ? -1.0 : 1.0;
  }

  const double ax = negative ? -x : x;

  if (LIBC_UNLIKELY(x_abs <= 0x3fe1'9999'9999'999aULL)) { // |x| <= 0.55
    const double t = ax * ax;
    double r = TANH_POLY[9];
    r = fputil::multiply_add(r, t, TANH_POLY[8]);
    r = fputil::multiply_add(r, t, TANH_POLY[7]);
    r = fputil::multiply_add(r, t, TANH_POLY[6]);
    r = fputil::multiply_add(r, t, TANH_POLY[5]);
    r = fputil::multiply_add(r, t, TANH_POLY[4]);
    r = fputil::multiply_add(r, t, TANH_POLY[3]);
    r = fputil::multiply_add(r, t, TANH_POLY[2]);
    r = fputil::multiply_add(r, t, TANH_POLY[1]);
    r = fputil::multiply_add(r, t, TANH_POLY[0]);
    const double res = ax * r;
    return negative ? -res : res;
  }

  if (x_abs < 0x4034'0000'0000'0000ULL) { // |x| < 20
    const double e = math::exp(2.0 * ax);
    const double res = 1.0 - 2.0 / (e + 1.0);
    return negative ? -res : res;
  }

  return negative ? -1.0 : 1.0;
}

} // namespace math
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_MATH_TANH_H
