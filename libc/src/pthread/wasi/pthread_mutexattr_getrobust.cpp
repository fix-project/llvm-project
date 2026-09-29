#include "src/pthread/pthread_mutexattr_getrobust.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutexattr_getrobust,
                   (const pthread_mutexattr_t *__restrict attr,
                    int *__restrict robust)) {
  if (attr == nullptr || robust == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  *robust = static_cast<int>((*attr >> 2) & 0x1u);
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
