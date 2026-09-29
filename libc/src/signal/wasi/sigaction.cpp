#include "src/signal/sigaction.h"

#include "signal_utils.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, sigaction,
                   (int signum, const struct sigaction *act,
                    struct sigaction *oldact)) {
  if (!wasi::is_valid_signum(signum)) {
    libc_errno = EINVAL;
    return -1;
  }
  struct sigaction &slot = wasi::signal_state().actions[signum];
  if (oldact != nullptr)
    *oldact = slot;
  if (act != nullptr)
    slot = *act;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
