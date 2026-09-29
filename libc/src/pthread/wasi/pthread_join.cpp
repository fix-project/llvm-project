#include "src/pthread/pthread_join.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_join, (pthread_t thread, void **retval)) {
  // WASI is single-threaded: no joinable threads exist.
  (void)thread;
  (void)retval;
  return ESRCH;
}

} // namespace LIBC_NAMESPACE_DECL
