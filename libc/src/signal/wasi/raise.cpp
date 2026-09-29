#include "src/signal/raise.h"

#include "signal_utils.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, raise, (int sig)) {
  // WASI has no asynchronous signal delivery, but a signal sent to the
  // calling process can be delivered synchronously: run the installed
  // handler or apply the default disposition.
  if (sig < 1 || sig > NSIG - 1) {
    libc_errno = EINVAL;
    return -1;
  }
  wasi::deliver_self(sig);
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
