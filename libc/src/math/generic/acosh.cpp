//===-- Double-precision acosh function -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/math/acosh.h"
#include "src/__support/FPUtil/FEnvImpl.h"
#include "src/__support/FPUtil/FPBits.h"
#include "src/__support/common.h"
#include "src/__support/math/log.h"
#include "src/__support/math/log1p.h"
#include "src/__support/math/sqrt.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(double, acosh, (double x)) {
  using FPBits = fputil::FPBits<double>;
  if (FPBits(x).is_nan())
    return x + x;
  if (x < 1.0) {
    fputil::set_errno_if_required(EDOM);
    fputil::raise_except_if_required(FE_INVALID);
    return FPBits::quiet_nan().get_val();
  }
  if (x == 1.0)
    return 0.0;
  if (x >= 0x1p26)
    return math::log(x) + 0x1.62e42fefa39efp-1;
  if (x >= 2.0)
    return math::log(2.0 * x - 1.0 / (x + math::sqrt(x * x - 1.0)));
  double t = x - 1.0;
  return math::log1p(t + math::sqrt(t * t + 2.0 * t));
}

} // namespace LIBC_NAMESPACE_DECL
