#include "src/pthread/pthread_cond_wait.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_cond_wait,
                   (pthread_cond_t *__restrict cond,
                    pthread_mutex_t *__restrict mutex)) {
  // WASI is single-threaded: a condition wait can never be satisfied.
  (void)cond;
  (void)mutex;
  libc_errno = EPERM;
  return EPERM;
}

} // namespace LIBC_NAMESPACE_DECL
