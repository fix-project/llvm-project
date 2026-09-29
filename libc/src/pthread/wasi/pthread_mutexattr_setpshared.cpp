#include "src/pthread/pthread_mutexattr_setpshared.h"

#include "src/__support/common.h"
#include "src/__support/libc_errno.h"
#include "src/__support/macros/config.h"
#include <pthread.h>

namespace LIBC_NAMESPACE_DECL {

LLVM_LIBC_FUNCTION(int, pthread_mutexattr_setpshared,
                   (pthread_mutexattr_t *__restrict attr, int pshared)) {
  if (attr == nullptr || (pshared != PTHREAD_PROCESS_PRIVATE &&
                          pshared != PTHREAD_PROCESS_SHARED)) {
    return EINVAL;
  }
  if (pshared == PTHREAD_PROCESS_SHARED)
    return ENOTSUP;
  *attr = (*attr & ~0x8u) | (static_cast<unsigned>(pshared) << 3);
  return 0;
}

} // namespace LIBC_NAMESPACE_DECL
