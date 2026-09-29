#include "src/pthread/pthread_spin_init.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_spin_init, (pthread_spinlock_t * lock, int pshared)) {
  (void)pshared;
  if (lock == nullptr) {
    return EINVAL;
  }
  lock->__lockword = 0;
  lock->__owner = 0;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
