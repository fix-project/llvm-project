#include "src/pthread/pthread_rwlock_timedrdlock.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlock_timedrdlock,
                   (pthread_rwlock_t *__restrict rwlock,
                    const struct timespec *__restrict abstime)) {
  (void)abstime;
  if (rwlock == nullptr) {
    return EINVAL;
  }
  if (rwlock->__raw.__state == 2) {
    return ETIMEDOUT;
  }
  rwlock->__raw.__state = 1;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
