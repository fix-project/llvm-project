#include "src/pthread/pthread_rwlockattr_getkind_np.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_rwlockattr_getkind_np,
                   (const pthread_rwlockattr_t *__restrict attr,
                    int *__restrict pref)) {
  if (attr == nullptr || pref == nullptr) {
    libc_errno = EINVAL;
    return EINVAL;
  }
  *pref = attr->pref;
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
