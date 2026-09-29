//===-- Implementation of cfmakeraw ---------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "src/termios/cfmakeraw.h"

#include "src/__support/common.h"

#include <termios.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(void, cfmakeraw, (struct termios * termios_p)) {
  termios_p->c_iflag &=
      ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  termios_p->c_oflag &= ~OPOST;
  termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  termios_p->c_cflag &= ~(CSIZE | PARENB);
  termios_p->c_cflag |= CS8;
  termios_p->c_cc[VMIN] = 1;
  termios_p->c_cc[VTIME] = 0;
}

} // namespace LIBC_NAMESPACE_DECL
