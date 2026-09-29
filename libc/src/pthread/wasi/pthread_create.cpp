#include "src/pthread/pthread_create.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_create,
                   (pthread_t *__restrict thread,
                    const pthread_attr_t *__restrict attr,
                    __pthread_start_t func, void *arg)) {
  // WASI is single-threaded: no additional threads can be created.
  (void)thread;
  (void)attr;
  (void)func;
  (void)arg;
  return ENOTSUP;
}

} // namespace LIBC_NAMESPACE_DECL
