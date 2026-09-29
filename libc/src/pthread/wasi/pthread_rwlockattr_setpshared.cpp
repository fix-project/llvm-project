#include "src/pthread/pthread_rwlockattr_setpshared.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlockattr_setpshared,
                   (pthread_rwlockattr_t * attr, int pshared)) {
  if (attr == nullptr || (pshared != PTHREAD_PROCESS_PRIVATE &&
                          pshared != PTHREAD_PROCESS_SHARED)) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  attr->pshared = pshared;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
