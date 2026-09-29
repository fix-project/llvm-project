#include "src/pthread/pthread_spin_unlock.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_spin_unlock, (pthread_spinlock_t * lock)) {
  if (lock == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  if (!lock->__lockword) {
    libc_errno = EPERM;
    return EPERM;
  }
  lock->__lockword = 0;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
