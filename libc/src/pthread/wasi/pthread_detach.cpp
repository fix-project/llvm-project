#include "src/pthread/pthread_detach.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_detach, (pthread_t thread)) {
  // WASI is single-threaded: no detachable threads exist.
  (void)thread;
  libc_errno = ESRCH;
  return ESRCH;
}

} // namespace LIBC_NAMESPACE_DECL
