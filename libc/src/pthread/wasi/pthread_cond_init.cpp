#include "src/pthread/pthread_cond_init.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_cond_init,
                   (pthread_cond_t *__restrict cond,
                    const pthread_condattr_t *__restrict attr)) {
  (void)attr;
  if (cond == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  *cond = {};
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
