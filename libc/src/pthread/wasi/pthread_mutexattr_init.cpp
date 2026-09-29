#include "src/pthread/pthread_mutexattr_init.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutexattr_init,
                   (pthread_mutexattr_t * attr)) {
  if (attr == nullptr) {
    return EINVAL;
  }
  *attr = 0;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
