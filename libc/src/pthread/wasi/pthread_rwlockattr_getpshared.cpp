#include "src/pthread/pthread_rwlockattr_getpshared.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlockattr_getpshared,
                   (const pthread_rwlockattr_t *attr, int *__restrict pshared)) {
  if (attr == nullptr || pshared == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  *pshared = attr->pshared;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
