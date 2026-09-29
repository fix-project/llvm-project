#include "src/pthread/pthread_mutex_init.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutex_init,
                   (pthread_mutex_t * mutex,
                    const pthread_mutexattr_t *__restrict attr)) {
  if (mutex == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  mutex->__locked = 0;
  mutex->__lock_count = 0;
  mutex->__owner = 0;
  mutex->__recursive = 0;
  mutex->__error_checking = 0;
  mutex->__robust = 0;
  mutex->__pshared = 0;
  mutex->__priority_inherit = 0;
  if (attr != nullptr) {
    const unsigned a = *attr;
    const int type = static_cast<int>(a & 0x3u);
    mutex->__recursive = (type == PTHREAD_MUTEX_RECURSIVE) ? 1u : 0u;
    mutex->__error_checking = (type == PTHREAD_MUTEX_ERRORCHECK) ? 1u : 0u;
    mutex->__robust = static_cast<unsigned>((a >> 2) & 0x1u);
    mutex->__pshared = static_cast<unsigned>((a >> 3) & 0x1u);
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
