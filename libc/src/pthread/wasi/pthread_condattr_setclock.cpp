#include "src/pthread/pthread_condattr_setclock.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_condattr_setclock,
                   (pthread_condattr_t * attr, clockid_t clock_id)) {
  if (attr == nullptr) {
    return EINVAL;
  }
  attr->clock = clock_id;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
