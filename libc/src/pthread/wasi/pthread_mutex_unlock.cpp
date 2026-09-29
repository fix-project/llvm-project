#include "src/pthread/pthread_mutex_unlock.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutex_unlock, (pthread_mutex_t * mutex)) {
  if (mutex == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  if (!mutex->__locked) {
    libc_errno = EPERM;
    return EPERM;
  }
  if (mutex->__recursive && mutex->__lock_count > 1) {
    --mutex->__lock_count;
    return 0;
  }
  mutex->__locked = 0;
  mutex->__lock_count = 0;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
