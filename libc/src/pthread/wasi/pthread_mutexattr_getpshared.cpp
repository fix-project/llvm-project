#include "src/pthread/pthread_mutexattr_getpshared.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutexattr_getpshared,
                   (const pthread_mutexattr_t *__restrict attr,
                    int *__restrict pshared)) {
  if (attr == nullptr || pshared == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  *pshared = static_cast<int>((*attr >> 3) & 0x1u);
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
