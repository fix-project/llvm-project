#include "src/pthread/pthread_mutex_destroy.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutex_destroy, (pthread_mutex_t * mutex)) {
  if (mutex == nullptr) {
    return EINVAL;
  }
  if (mutex->__locked) {
    return EBUSY;
  }
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
