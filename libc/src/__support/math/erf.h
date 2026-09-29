//===-- Inline erf function -----------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_SRC___SUPPORT_MATH_ERF_H
#define LLVM_LIBC_SRC___SUPPORT_MATH_ERF_H

#include "exp.h"
#include "src/__support/FPUtil/FEnvImpl.h"
#include "src/__support/FPUtil/FPBits.h"
#include "src/__support/FPUtil/multiply_add.h"
#include "src/__support/macros/config.h"
#include "src/__support/macros/optimization.h" // LIBC_UNLIKELY

namespace LIBC_NAMESPACE_DECL {
namespace math {

LIBC_INLINE double erf(double x) {
  static constexpr double ERF_POLY[] = {
    0x1.20dd750429b69p+0,
    -0x1.812746b035782p-2,
    0x1.ce2f219db6980p-4,
    -0x1.b82ce1e47c000p-6,
    0x1.565b869d00000p-8,
    -0x1.c01b7ec000000p-11,
    0x1.f6fc200000000p-14,
    -0x1.c050000000000p-17,
  };

  static constexpr double ERF_Q0[] = {
    0x1.b5d8780f956b3p-2,
    -0x1.17c4e3f17c20ap-2,
    0x1.3c27283c32b21p-3,
    -0x1.44837f88c6255p-4,
    0x1.33cad0ef788cdp-5,
    -0x1.10fcf1e17c444p-6,
    0x1.c8cb957a1fffep-8,
    -0x1.6af24ae022221p-9,
    0x1.13526443bbbbdp-10,
    -0x1.908a3f7777778p-12,
    0x1.18509f3333333p-13,
    -0x1.795d99999999ep-15,
    0x1.ee54000000006p-17,
    -0x1.55f3333333333p-18,
    0x1.9a66666666666p-20,
  };
  static constexpr double ERF_Q1[] = {
    0x1.d94446d627933p-3,
    -0x1.6a70d2bb37fa7p-4,
    0x1.0615670e25ed2p-5,
    -0x1.6883f9901bd04p-7,
    0x1.da595560d9310p-9,
    -0x1.2bd2529f74605p-10,
    0x1.6d7744818dccfp-12,
    -0x1.aed77386cb093p-14,
    0x1.ec76de142382cp-16,
    -0x1.118aa3477801dp-17,
    0x1.27bc9a5576422p-19,
    -0x1.356305865434fp-21,
    0x1.3ee47a83dc77ap-23,
    -0x1.6e65a0a1aca80p-25,
    0x1.655aa9b9d9378p-27,
  };
  static constexpr double ERF_Q2[] = {
    0x1.023fe1060c504p-3,
    -0x1.c3379ff37290cp-6,
    0x1.81bb28a8027cep-8,
    -0x1.431311af3a4aap-10,
    0x1.09688e037e191p-12,
    -0x1.ac20ff3f060a4p-15,
    0x1.535075cdf2df8p-17,
    -0x1.08755e2e145bap-19,
    0x1.95b4c9a43b02bp-22,
    -0x1.32d0bef2135f7p-24,
    0x1.c8b2fe9903b9bp-27,
    -0x1.4720fe107746ap-29,
    0x1.da2c7d0858317p-32,
    -0x1.ae90ba0104170p-34,
    0x1.2e11fcbf1c75ep-36,
  };

  using FPBits = fputil::FPBits<double>;
  const uint64_t bits = FPBits(x).uintval();
  const uint64_t x_abs = bits & 0x7fff'ffff'ffff'ffffULL;
  const bool negative = bits >> 63;

  // |x| < 2^-28: erf(x) ~= 2/sqrt(pi) * x
  if (LIBC_UNLIKELY(x_abs < 0x3e30'0000'0000'0000ULL))
    return x * 0x1.20dd67c1fda7ep+0; // 2/sqrt(pi)

  // nan
  if (LIBC_UNLIKELY(x_abs > 0x7ff0'0000'0000'0000ULL)) {
    fputil::raise_except_if_required(FE_INVALID);
    return FPBits::quiet_nan().get_val();
  }

  // |x| >= 5.73 (includes inf): erf rounds to +-1
  if (LIBC_UNLIKELY(x_abs >= 0x4016'eb85'1eb8'51ecULL))
    return negative ? -1.0 : 1.0;

  const double ax = negative ? -x : x;

  if (LIBC_UNLIKELY(x_abs <= 0x3fe0'0000'0000'0000ULL)) { // |x| <= 0.5
    const double t = ax * ax;
    double r = ERF_POLY[7];
    r = fputil::multiply_add(r, t, ERF_POLY[6]);
    r = fputil::multiply_add(r, t, ERF_POLY[5]);
    r = fputil::multiply_add(r, t, ERF_POLY[4]);
    r = fputil::multiply_add(r, t, ERF_POLY[3]);
    r = fputil::multiply_add(r, t, ERF_POLY[2]);
    r = fputil::multiply_add(r, t, ERF_POLY[1]);
    r = fputil::multiply_add(r, t, ERF_POLY[0]);
    const double res = ax * r;
    return negative ? -res : res;
  }

  double q;
if (x_abs < 0x3ff8'0000'0000'0000ULL) { // |x| < 1.5
    const double s = ax - 0x1.0000000000000p+0;
    double r = ERF_Q0[14];
    r = fputil::multiply_add(r, s, ERF_Q0[13]);
    r = fputil::multiply_add(r, s, ERF_Q0[12]);
    r = fputil::multiply_add(r, s, ERF_Q0[11]);
    r = fputil::multiply_add(r, s, ERF_Q0[10]);
    r = fputil::multiply_add(r, s, ERF_Q0[9]);
    r = fputil::multiply_add(r, s, ERF_Q0[8]);
    r = fputil::multiply_add(r, s, ERF_Q0[7]);
    r = fputil::multiply_add(r, s, ERF_Q0[6]);
    r = fputil::multiply_add(r, s, ERF_Q0[5]);
    r = fputil::multiply_add(r, s, ERF_Q0[4]);
    r = fputil::multiply_add(r, s, ERF_Q0[3]);
    r = fputil::multiply_add(r, s, ERF_Q0[2]);
    r = fputil::multiply_add(r, s, ERF_Q0[1]);
    r = fputil::multiply_add(r, s, ERF_Q0[0]);
    q = r;
} else if (x_abs < 0x4008'0000'0000'0000ULL) { // |x| < 3.0
    const double s = ax - 0x1.2000000000000p+1;
    double r = ERF_Q1[14];
    r = fputil::multiply_add(r, s, ERF_Q1[13]);
    r = fputil::multiply_add(r, s, ERF_Q1[12]);
    r = fputil::multiply_add(r, s, ERF_Q1[11]);
    r = fputil::multiply_add(r, s, ERF_Q1[10]);
    r = fputil::multiply_add(r, s, ERF_Q1[9]);
    r = fputil::multiply_add(r, s, ERF_Q1[8]);
    r = fputil::multiply_add(r, s, ERF_Q1[7]);
    r = fputil::multiply_add(r, s, ERF_Q1[6]);
    r = fputil::multiply_add(r, s, ERF_Q1[5]);
    r = fputil::multiply_add(r, s, ERF_Q1[4]);
    r = fputil::multiply_add(r, s, ERF_Q1[3]);
    r = fputil::multiply_add(r, s, ERF_Q1[2]);
    r = fputil::multiply_add(r, s, ERF_Q1[1]);
    r = fputil::multiply_add(r, s, ERF_Q1[0]);
    q = r;
} else if (x_abs < 0x4016'eb85'1eb8'51ecULL) { // |x| < 5.73
    const double s = ax - 0x1.175c28f5c28f6p+2;
    double r = ERF_Q2[14];
    r = fputil::multiply_add(r, s, ERF_Q2[13]);
    r = fputil::multiply_add(r, s, ERF_Q2[12]);
    r = fputil::multiply_add(r, s, ERF_Q2[11]);
    r = fputil::multiply_add(r, s, ERF_Q2[10]);
    r = fputil::multiply_add(r, s, ERF_Q2[9]);
    r = fputil::multiply_add(r, s, ERF_Q2[8]);
    r = fputil::multiply_add(r, s, ERF_Q2[7]);
    r = fputil::multiply_add(r, s, ERF_Q2[6]);
    r = fputil::multiply_add(r, s, ERF_Q2[5]);
    r = fputil::multiply_add(r, s, ERF_Q2[4]);
    r = fputil::multiply_add(r, s, ERF_Q2[3]);
    r = fputil::multiply_add(r, s, ERF_Q2[2]);
    r = fputil::multiply_add(r, s, ERF_Q2[1]);
    r = fputil::multiply_add(r, s, ERF_Q2[0]);
    q = r;
  } else {
    q = 0.0;
  }

  const double res = 1.0 - math::exp(-ax * ax) * q;
  return negative ? -res : res;
}

} // namespace math
} // namespace LIBC_NAMESPACE_DECL

#endif // LLVM_LIBC_SRC___SUPPORT_MATH_ERF_H
