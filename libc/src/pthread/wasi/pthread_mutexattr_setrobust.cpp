#include "src/pthread/pthread_mutexattr_setrobust.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutexattr_setrobust,
                   (pthread_mutexattr_t * attr, int robust)) {
  if (attr == nullptr || (robust != PTHREAD_MUTEX_STALLED &&
                          robust != PTHREAD_MUTEX_ROBUST)) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  *attr = (*attr & ~0x4u) | (static_cast<unsigned>(robust) << 2);
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
