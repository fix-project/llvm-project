#include "src/pthread/pthread_spin_destroy.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_spin_destroy, (pthread_spinlock_t * lock)) {
  (void)lock;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
