#include "src/pthread/pthread_spin_lock.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_spin_lock, (pthread_spinlock_t * lock)) {
  if (lock == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  if (lock->__lockword) {
    libc_errno = EDEADLK;
    return EDEADLK;
  }
  lock->__lockword = 1;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
