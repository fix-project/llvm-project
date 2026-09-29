#include "src/signal/sigfillset.h"

#include "signal_utils.h"
#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, sigfillset, (sigset_t * set)) {
  if (set == nullptr) {
    libc_errno = EINVAL;
    return -1;
  }
  *set = wasi::full_set();
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
