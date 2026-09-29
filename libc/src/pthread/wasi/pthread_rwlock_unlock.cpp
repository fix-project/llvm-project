#include "src/pthread/pthread_rwlock_unlock.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlock_unlock, (pthread_rwlock_t * rwlock)) {
  if (rwlock == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  rwlock->__raw.__state = 0;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
