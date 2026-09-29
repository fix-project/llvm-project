#include "src/pthread/pthread_rwlock_clockwrlock.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlock_clockwrlock,
                   (pthread_rwlock_t *__restrict rwlock, clockid_t clock_id,
                    const struct timespec *__restrict abstime)) {
  (void)clock_id;
  (void)abstime;
  if (rwlock == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  if (rwlock->__raw.__state != 0) {
    libc_errno = ETIMEDOUT;
    return ETIMEDOUT;
  }
  rwlock->__raw.__state = 2;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
