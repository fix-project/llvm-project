#include "src/signal/sigaddset.h"

#include "signal_utils.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, sigaddset, (sigset_t * set, int signum)) {
  if (set != nullptr && wasi::add_signal(*set, signum))
    return 0;
  libc_errno = EINVAL;
  return -1;
}

} // namespace LIBC_NAMESPACE_DECL
