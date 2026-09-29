#include "src/pthread/pthread_mutexattr_gettype.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutexattr_gettype,
                   (const pthread_mutexattr_t *__restrict attr,
                    int *__restrict type)) {
  if (attr == nullptr || type == nullptr) {
    return EINVAL;
  }
  *type = static_cast<int>(*attr & 0x3u);
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
