#include "src/pthread/pthread_rwlockattr_init.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlockattr_init,
                   (pthread_rwlockattr_t * attr)) {
  if (attr == nullptr) {
    return EINVAL;
  }
  attr->pshared = PTHREAD_PROCESS_PRIVATE;
  attr->pref = PTHREAD_RWLOCK_PREFER_READER_NP;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
