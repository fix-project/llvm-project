//===-- Double-precision complementary error function --------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// The rational approximations below originate in FreeBSD's s_erf.c:
// Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
// Developed at SunPro, a Sun Microsystems, Inc. business.
// Permission to use, copy, modify, and distribute this software is freely
// granted, provided that this notice is preserved.
//
//===----------------------------------------------------------------------===//

#include "src/math/erfc.h"
#include "src/__support/FPUtil/FPBits.h"
#include "src/__support/common.h"
#include "src/__support/math/erf.h"
#include "src/__support/math/exp.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(double, erfc, (double x)) {
  using FPBits = fputil::FPBits<double>;
  FPBits bits(x);
  if (bits.is_nan())
    return x + x;
  if (bits.is_inf())
    return bits.is_neg() ? 2.0 : 0.0;

  double ax = bits.abs().get_val();
  if (ax < 1.25)
    return 1.0 - math::erf(x);
  if (ax >= 28.0)
    return bits.is_neg() ? 2.0 : 0.0;

  double s = 1.0 / (ax * ax);
  double r, q;
  if (ax < 1.0 / 0.35) {
    constexpr double ra[] = {
        -9.86494403484714822705e-03, -6.93858572707181764372e-01,
        -1.05586262253232909814e+01, -6.23753324503260060396e+01,
        -1.62396669462573470355e+02, -1.84605092906711035994e+02,
        -8.12874355063065934246e+01, -9.81432934416914548592e+00};
    constexpr double sa[] = {
        1.96512716674392571292e+01, 1.37657754143519042600e+02,
        4.34565877475229228821e+02, 6.45387271733267880336e+02,
        4.29008140027567833386e+02, 1.08635005541779435134e+02,
        6.57024977031928170135e+00, -6.04244152148580987438e-02};
    r = ra[0] +
        s * (ra[1] +
             s * (ra[2] +
                  s * (ra[3] +
                       s * (ra[4] + s * (ra[5] + s * (ra[6] + s * ra[7]))))));
    q = 1.0 +
        s * (sa[0] +
             s * (sa[1] +
                  s * (sa[2] +
                       s * (sa[3] +
                            s * (sa[4] +
                                 s * (sa[5] + s * (sa[6] + s * sa[7])))))));
  } else {
    constexpr double rb[] = {
        -9.86494292470009928597e-03, -7.99283237680523006574e-01,
        -1.77579549177547519889e+01, -1.60636384855821916062e+02,
        -6.37566443368389627722e+02, -1.02509513161107724954e+03,
        -4.83519191608651397019e+02};
    constexpr double sb[] = {
        3.03380607434824582924e+01, 3.25792512996573918826e+02,
        1.53672958608443695994e+03, 3.19985821950859553908e+03,
        2.55305040643316442583e+03, 4.74528541206955367215e+02,
        -2.24409524465858183362e+01};
    r = rb[0] +
        s * (rb[1] +
             s * (rb[2] + s * (rb[3] + s * (rb[4] + s * (rb[5] + s * rb[6])))));
    q = 1.0 +
        s * (sb[0] +
             s * (sb[1] +
                  s * (sb[2] +
                       s * (sb[3] + s * (sb[4] + s * (sb[5] + s * sb[6]))))));
  }

  // Split x*x so the small correction remains accurate in the exponential.
  double z = FPBits(bits.abs().uintval() & 0xffff'ffff'0000'0000ULL).get_val();
  double tail =
      math::exp(-z * z - 0.5625) * math::exp((z - ax) * (z + ax) + r / q) / ax;
  return bits.is_neg() ? 2.0 - tail : tail;
}

} // namespace LIBC_NAMESPACE_DECL
