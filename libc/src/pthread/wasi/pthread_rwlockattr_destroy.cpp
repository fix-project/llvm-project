#include "src/pthread/pthread_rwlockattr_destroy.h"

#include "src/__support/common.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlockattr_destroy,
                   (pthread_rwlockattr_t * attr)) {
  (void)attr;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
