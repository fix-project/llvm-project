#include "src/pthread/pthread_rwlock_destroy.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlock_destroy, (pthread_rwlock_t * rwlock)) {
  (void)rwlock;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
