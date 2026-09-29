#include "src/pthread/pthread_barrier_init.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_barrier_init,
                   (pthread_barrier_t * b,
                    const pthread_barrierattr_t *__restrict attr,
                    unsigned count)) {
  (void)attr;
  if (b == nullptr || count == 0) {
    return EINVAL;
  }
  b->expected = count;
  b->waiting = 0;
  b->blocking = false;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
