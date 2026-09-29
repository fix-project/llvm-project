#include "src/pthread/pthread_condattr_getclock.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_condattr_getclock,
                   (const pthread_condattr_t *__restrict attr,
                    clockid_t *__restrict clock_id)) {
  if (attr == nullptr || clock_id == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  *clock_id = attr->clock;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
