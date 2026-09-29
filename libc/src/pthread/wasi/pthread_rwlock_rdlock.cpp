#include "src/pthread/pthread_rwlock_rdlock.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlock_rdlock, (pthread_rwlock_t * rwlock)) {
  if (rwlock == nullptr) {
    return EINVAL;
  }
  if (rwlock->__raw.__state == 2) {
    return EDEADLK;
  }
  rwlock->__raw.__state = 1;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
