#include "src/pthread/pthread_barrier_destroy.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_barrier_destroy, (pthread_barrier_t * b)) {
  (void)b;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
