#include "src/pthread/pthread_rwlock_init.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlock_init,
                   (pthread_rwlock_t * rwlock,
                    const pthread_rwlockattr_t *__restrict attr)) {
  (void)attr;
  if (rwlock == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  rwlock->__raw.__state = 0;
  rwlock->__writer_tid = 0;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
