#include "src/pthread/pthread_condattr_init.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_condattr_init, (pthread_condattr_t * attr)) {
  if (attr == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  attr->clock = 0; // CLOCK_REALTIME
  attr->pshared = PTHREAD_PROCESS_PRIVATE;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
