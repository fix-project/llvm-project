#include "src/pthread/pthread_mutex_trylock.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutex_trylock, (pthread_mutex_t * mutex)) {
  if (mutex == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  if (mutex->__locked) {
    if (mutex->__recursive) {
      ++mutex->__lock_count;
      return 0;
    }
    libc_errno = EBUSY;
    return EBUSY;
  }
  mutex->__locked = 1;
  mutex->__lock_count = 1;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
