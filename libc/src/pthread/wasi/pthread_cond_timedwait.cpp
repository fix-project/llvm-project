#include "src/pthread/pthread_cond_timedwait.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_cond_timedwait,
                   (pthread_cond_t *__restrict cond,
                    pthread_mutex_t *__restrict mutex,
                    const struct timespec *__restrict abstime)) {
  (void)cond;
  (void)mutex;
  (void)abstime;
  return ETIMEDOUT;
}

} // namespace LIBC_NAMESPACE_DECL
