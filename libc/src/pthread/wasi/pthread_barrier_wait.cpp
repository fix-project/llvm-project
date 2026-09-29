#include "src/pthread/pthread_barrier_wait.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_barrier_wait, (pthread_barrier_t * b)) {
  // WASI is single-threaded: a barrier with a count above one can never
  // complete. A single-party barrier completes immediately.
  if (b == nullptr) {
    return EINVAL;
  }
  if (b->expected > 1) {
    return EDEADLK;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
