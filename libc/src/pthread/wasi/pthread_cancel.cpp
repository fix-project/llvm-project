#include "src/pthread/pthread_cancel.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_cancel, (pthread_t thread)) {
  // WASI is single-threaded: no cancellable threads exist.
  (void)thread;
  return ESRCH;
}

} // namespace LIBC_NAMESPACE_DECL
